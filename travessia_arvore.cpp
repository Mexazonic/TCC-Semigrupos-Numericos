#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <cpuid.h>
#include <cilk/cilk_api.h>

using namespace std;
using namespace std::chrono;

#include "travessia_arvore.hpp"
#include<unistd.h>


void walk_children_stack(SemigrupoNumerico s, results_type res)
{
    unsigned long int nbr;
    SemigrupoNumerico data[MAX_GENUS-1], *stack[MAX_GENUS], *current;
    SemigrupoNumerico **stack_pointer = stack + 1;

    for (ind_t i=1; i<MAX_GENUS; i++) stack[i] = &(data[i-1]);

    stack[0] = &s;
    while (stack_pointer != stack)
    {
        --stack_pointer;
        current = *stack_pointer;
        if (current->genero < MAX_GENUS - 1)
    {
        nbr = 0;
        auto it = generator_iter<CHILDREN>(*current);
        while (it.move_next())
        {
            stack_pointer[0] = stack_pointer[1];
            remove_generator(**stack_pointer, *current, it.get_gen());
            stack_pointer++;
            nbr++;
        }
        *stack_pointer = current;
        res[current->genero] += nbr;
    }
        else
    {
        auto it = generator_iter<CHILDREN>(*current);
        res[current->genero] += it.count();
    }
    }
}


ResultsReducer cilk_results;

#ifndef STACK_BOUND
#define STACK_BOUND 11
#endif

void walk_children(const SemigrupoNumerico s){
    unsigned long int nbr = 0;

    if (s.genero < MAX_GENUS - STACK_BOUND){
        
        auto it = generator_iter<CHILDREN>(s);
        while (it.move_next()){
            cilk_spawn walk_children(remove_generator(s, it.get_gen()));
            nbr++;
        }
        cilk_results[s.genero] += nbr;
    }

    else
        walk_children_stack(s, cilk_results.get_array());
}




void walk_children_stack(SemigrupoNumerico s, ind_t bound, results_type res)
{
    unsigned long int nbr;
    SemigrupoNumerico data[bound], *stack[bound], *current;
    SemigrupoNumerico **stack_pointer = stack + 1;

    for (ind_t i=1; i<bound; i++) stack[i] = &(data[i-1]); // Nathann's trick to avoid copy
    stack[0] = &s;
    while (stack_pointer != stack)
    {
        --stack_pointer;
        current = *stack_pointer;
        if (current->genero < bound - 1)
    {
        nbr = 0;
        auto it = generator_iter<CHILDREN>(*current);
        while (it.move_next())
        {
            // exchange top with top+1
            stack_pointer[0] = stack_pointer[1];
            remove_generator(**stack_pointer, *current, it.get_gen());
            stack_pointer++;
            nbr++;
        }
        *stack_pointer = current;
        res[current->genero] += nbr;
    }
        else
    {
        auto it = generator_iter<CHILDREN>(*current);
        res[current->genero] += it.count();
    }
    }
}

void walk_children(const SemigrupoNumerico &s, ind_t bound) {
  unsigned long int nbr = 0;

    if (s.genero < bound - STACK_BOUND){
        auto it = generator_iter<CHILDREN>(s);
        
        while (it.move_next()){
	        cilk_spawn walk_children(remove_generator(s, it.get_gen()), bound);
	        nbr++;
	    }
    
        cilk_results[s.genero] += nbr;
    }
  
    else
        walk_children_stack(s, bound, cilk_results.get_array());
}


#ifdef TBB
#include <tbb/scalable_allocator.h>
cilk::reducer_list_append<SemigrupoNumerico, tbb::scalable_allocator<SemigrupoNumerico>> cilk_list_results;
#else
cilk::reducer_list_append<SemigrupoNumerico> cilk_list_results;
#endif

void list_children(const SemigrupoNumerico &s, ind_t bound) {
    if (s.genero < bound) {
        auto it = generator_iter<CHILDREN>(s);
        while (it.move_next())
            cilk_spawn list_children(remove_generator(s, it.get_gen()), bound);
     }

    else cilk_list_results.push_back(s);
}


static void show_usage(string name){ cerr << "Usage: " << name << " [-n <proc_number>] " << endl;}


int main(int argc, char **argv)
{
    SemigrupoNumerico N;
    unsigned long int total = 0;
    string nproc = "0";

    if (argc != 1 and argc != 3) { show_usage(argv[0]); return 1; }
    if (argc == 3){
        if (string(argv[1]) != "-n")  { show_usage(argv[0]); return 1; }
        nproc = argv[2];
    }

    unsigned int ax, bx, cx, dx;
    if (!__get_cpuid(0x00000001, &ax, &bx, &cx, &dx)) {
        cerr << "Unable to determine the processor type !" << endl;
        return EXIT_FAILURE;
    }
    if (!(cx & bit_SSSE3)) {
        cerr << "This programm require sse3 instructions set !" << endl;
        return EXIT_FAILURE;
    }
    
    if (!(cx & bit_POPCNT)){
        cerr << "This programm require popcount instruction !" << endl;
        return EXIT_FAILURE;
    }

    if (__cilkrts_set_param("nworkers", nproc.c_str() ) != __CILKRTS_SET_PARAM_SUCCESS)
        cerr << "Failed to set the number of Cilk workers" << endl;

    
    cout << "Computação da sequencia ng semigrupos numéricos de gênero <= " << MAX_GENUS << " using " << __cilkrts_get_nworkers() << " workers" << endl;
    cout << "Sizeof(Semigrupo) = " << sizeof(SemigrupoNumerico) << endl;
    auto begin = high_resolution_clock::now();
    Root(N);
    walk_children(N);
    auto end = high_resolution_clock::now();
    
    duration<double> ticks = end-begin;
    std::ofstream arquivo(std::to_string(MAX_GENUS) + "_output.txt");
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(arquivo.rdbuf());


    cout << endl << "============================" << endl << endl;
    cout << "0) 1\n";

    for (unsigned int i=0; i<MAX_GENUS; i++)
    {
        cout << i+1 << ") "<< cilk_results[i] << "\n";
        total += cilk_results[i];
    }
    cout << endl;
    cout << "Total = " << total <<
        ", computation time = " << std::setprecision(4) << ticks.count() << " s."  << endl;


    // Restaurar a saída padrão
    std::cout.rdbuf(coutbuf);

    // Fechar o arquivo
    arquivo.close();


    return EXIT_SUCCESS;
}
