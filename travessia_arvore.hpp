#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_list.h>
#include "semigrupo.hpp"

typedef unsigned long int results_type[MAX_GENUS];
void walk_children_stack(SemigrupoNumerico s, results_type res);
void walk_children_stack(SemigrupoNumerico s, ind_t bound, results_type res);

struct Results
{
  results_type values;
  inline Results() {reset();};
  inline void reset() {for(int i=0; i<MAX_GENUS; i++) values[i] = 0;};
};

class ResultsReducer
{
  struct Monoid: cilk::monoid_base<Results>
  {
    inline static void reduce (Results *left, Results *right){
      for(auto i=0; i<MAX_GENUS; i++) left->values[i] += right->values[i];
    }
  };
private:
  cilk::reducer<Monoid> imp_;
public:
  ResultsReducer() : imp_() {};
  inline unsigned long int & operator[](ind_t i) {
    return imp_.view().values[i];
  };
  inline results_type &get_array() {return imp_.view().values;}
  inline void reset() {imp_.view().reset();}
};

extern ResultsReducer cilk_results;

#ifdef TBB
#include <tbb/scalable_allocator.h>
extern cilk::reducer_list_append<SemigrupoNumerico, tbb::scalable_allocator<SemigrupoNumerico>> cilk_list_results;
#else
extern cilk::reducer_list_append<SemigrupoNumerico> cilk_list_results;
#endif


void walk_children(const SemigrupoNumerico s);
void walk_children(const SemigrupoNumerico &s, ind_t bound);
void list_children(const SemigrupoNumerico &s, ind_t bound);
