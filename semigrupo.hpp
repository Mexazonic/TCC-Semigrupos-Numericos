#ifndef MONOID_HPP
#define MONOID_HPP
#include <bits/stdc++.h>
#include <cstdint>
#include<unistd.h>
#include <x86intrin.h>
#include <cassert>
using namespace std;

#ifndef MAX_GENUS
#error "Please define the MAX_GENUS macro"
#endif
#define SIZE_BOUND (3*(MAX_GENUS-1))
#define NBLOCKS ((SIZE_BOUND+15) >> 4)
#define SIZE (NBLOCKS << 4)

typedef uint_fast64_t ind_t;
typedef uint8_t epi8 __attribute__ ((vector_size (16)));
typedef uint8_t dec_numbers[SIZE] __attribute__ ((aligned (16))); 
typedef epi8 dec_blocks[NBLOCKS];
typedef uint_fast64_t ind_t;
const epi8 zero = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const epi8 block1 = zero + 1;
const uint8_t m1 = 255;

//Require SSE 3
extern inline epi8 load_unaligned_epi8(const uint8_t *t){return (epi8) _mm_loadu_si128((__m128i *) (t)); };
extern inline epi8 shuffle_epi8(epi8 b, epi8 sh) { return (epi8) _mm_shuffle_epi8((__m128i) b, (__m128i) sh);}
extern inline int movemask_epi8(epi8 b) { return _mm_movemask_epi8((__m128i) b);};
// o compilador deve substituir chamadas para essa função pelo próprio código da função durante a compilação, em vez de gerar uma chamada de função separada.
inline __attribute__((always_inline))
void copy_blocks(dec_blocks &dst, dec_blocks const &src) {
        for (ind_t i=0; i<NBLOCKS; i++) dst[i] = src[i];
    }



const epi8 mask16[16] =  //EXTRAIR A POSIÇÃO DOS ELEMENTOS MINIMAIS DENTRO DE UM BLOCO DE 16 BYTES
  { {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1,m1},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m1} };

const epi8 shift16[16] = //EXTRAIR O DESLOCAMENTO DO ELEMENTO MINIMAL
  { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
    {m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14},
    {m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13},
    {m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12},
    {m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11},
    {m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10},
    {m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7, 8},
    {m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6, 7},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5, 6},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4, 5},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3, 4},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2, 3},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1, 2},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0, 1},
    {m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1,m1, 0} };




class SemigrupoNumerico {
    public:
        ind_t condutor;
        ind_t genero;
        ind_t multiplicidade;

        union{
            dec_numbers decs;
            dec_blocks blocks;
        };
};


class CHILDREN{};

template <class T> class generator_iter {
    private:
        const SemigrupoNumerico &s;
        unsigned int mask;
        ind_t iblock, gen, bound;

    public:
        generator_iter(const SemigrupoNumerico &s);
        bool move_next();
        uint8_t count();
        inline ind_t get_gen() const {return gen;};
};



void Root(SemigrupoNumerico &);
void print_epi8(epi8);
void print_semigrupo(const SemigrupoNumerico &);

template<> inline generator_iter<CHILDREN>::generator_iter(const SemigrupoNumerico &semigrupo): s(semigrupo), bound((semigrupo.condutor+semigrupo.multiplicidade+15) >> 4){

    epi8 block;
    iblock = semigrupo.condutor >> 4;
    block = semigrupo.blocks[iblock] & mask16[semigrupo.condutor & 0xF];
    mask = movemask_epi8(block == block1);
    gen = (iblock << 4) - 1;
};

// Função utilizada para poder gerar os semigrupos numéricos de cada um dos nós
template <class T> inline bool generator_iter<T>::move_next(){

    while (!mask){
        iblock++;
        if (iblock > bound) return false;
        gen = (iblock << 4) - 1;
        mask  = movemask_epi8(s.blocks[iblock] == block1);
    }
    
    unsigned char shift = __bsfd (mask) + 1; // Bit Scan Forward - bit menos significativo é usada para encontrar o índice do primeiro bit set (ativo) em um número inteiro
    gen += shift;
    mask >>= shift;
    return true;
};


//Ela é usada para contar o número de bits definidos como 1 em um valor de 32 bits (unsigned int).
template <class T> inline uint8_t generator_iter<T>::count() 
{
    uint8_t nbr = _mm_popcnt_u32(mask); // popcnt returns a 8 bits value
    for (ind_t ib = iblock+1; ib < bound; ib++)
        nbr += _mm_popcnt_u32(movemask_epi8(s.blocks[ib] == block1));
    
    return nbr;
};




inline __attribute__((always_inline))
void remove_generator(SemigrupoNumerico &__restrict__ dst, const SemigrupoNumerico &__restrict__ src, ind_t gen) {

    ind_t start_block, decal;
    epi8 block;

    assert(src.decs[gen] == 1);

    dst.condutor = gen + 1;
    dst.genero = src.genero + 1;
    dst.multiplicidade = (gen == src.multiplicidade) ? dst.condutor : src.multiplicidade;

    copy_blocks(dst.blocks, src.blocks); // 

    start_block = gen >> 4; //indice do block que contem x
    decal = gen & 0xF; //offset de x no bloco
    block = shuffle_epi8(src.blocks[0], shift16[decal]); // Quais elementos serão utilizados no array
    
    //Os elementos selecionados são copiados na mesma ordem para o vetor de destino, enquanto os elementos não selecionados são preenchidos com zeros.
    dst.blocks[start_block] -= ((block != zero) & block1); // GERAL QUE É DIFERENTE DE ZERO EU SUBTRAIO 1
    
    #if NBLOCKS >= 5

    #define CASE_UNROLL(i_loop)			\
    case i_loop : \
    dst.blocks[i_loop+1] -= (load_unaligned_epi8(srcblock) != zero) & block1; \
    srcblock += sizeof(epi8);

    {
    const uint8_t *srcblock = src.decs + sizeof(epi8) - decal;
    switch(start_block)
    {
        CASE_UNROLL(0);
    #if NBLOCKS > 2
        CASE_UNROLL(1);
    #endif
    #if NBLOCKS > 3
        CASE_UNROLL(2);
    #endif
    #if NBLOCKS > 4
        CASE_UNROLL(3);
    #endif
    #if NBLOCKS > 5
        CASE_UNROLL(4);
    #endif
    #if NBLOCKS > 6
        CASE_UNROLL(5);
    #endif
    #if NBLOCKS > 7
        CASE_UNROLL(6);
    #endif
    #if NBLOCKS > 8
        CASE_UNROLL(7);
    #endif
    #if NBLOCKS > 9
        CASE_UNROLL(8);
    #endif
    #if NBLOCKS > 10
        CASE_UNROLL(9);
    #endif
    #if NBLOCKS > 11
        CASE_UNROLL(10);
    #endif
    #if NBLOCKS > 12
        CASE_UNROLL(11);
    #endif
    #if NBLOCKS > 13
        CASE_UNROLL(12);
    #endif
    #if NBLOCKS > 14
        CASE_UNROLL(13);
    #endif
    #if NBLOCKS > 15
        CASE_UNROLL(14);
    #endif
    #if NBLOCKS > 16
    #error "Too many blocks"
    #endif
        }
    }
    #else
    #warning "Loop not unrolled"

    for (auto i=start_block+1; i<NBLOCKS; i++)
    {
        // The following won't work due to some alignment problem (bug in GCC 4.7.1?)
        // block = *((epi8*)(src.decs + ((i-start_block)<<4) - decal));
        block = load_unaligned_epi8(src.decs + ((i-start_block)<<4) - decal);
        dst.blocks[i] -= ((block != zero) & block1);
    }
    #endif

    assert(dst.decs[dst.condutor-1] == 0);
}





inline SemigrupoNumerico remove_generator(const SemigrupoNumerico &src, ind_t gen){
    SemigrupoNumerico dst;
    remove_generator(dst, src, gen);
    return dst;
}




#endif
