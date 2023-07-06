#include "semigrupo.hpp"
using namespace std;


void Root(SemigrupoNumerico &s){
    s.genero = 0;
    s.condutor = 1;
    s.multiplicidade = 1;
    epi8 block ={1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8};
    
    for(auto i=0;i<NBLOCKS; i++){
        s.blocks[i] = block;
        block = block + 8;
    }
}

void print_semigrupo(const SemigrupoNumerico &s) {
  
  unsigned int i;
  std::cout<<"multiplicidade = "<<s.multiplicidade<<", condutor = "<<s.condutor<<", genero = "<<s.genero<<", decomposicao = ";
  for (i=0; i<SIZE; i++) std::cout<<((int) s.decs[i])<<' ';
  std::cout<<std::endl;
}

void print_epi8(epi8 bl) {
  for (int i = 0; i < 16; i++) std::cout << static_cast<int>(bl[i]) << " ";
  cout << endl;
}

