#include "ompsearch.h"
#include "eval.h"
#include "board.h"
#include "transpositionTable.h"
#include <omp.h>
#include <random>


SearchResult ompSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, 
  float currEval, int depth, int turnPlayer){
  
  TranspositionTable t = TranspositionTable();
  std::uint64_t hash = Hash::initHash(pieces, playersInGame);
  
  int threadAmount = omp_get_max_threads();
  SearchResult r = SearchResult(false,-1);
  
  #pragma omp parallel num_threads(threadAmount) shared(t)
  {
    __uint128_t occ_copy = occupied;
    std::vector<__uint128_t> pieces_copy = pieces;
    
    SearchResult result = Search(&occ_copy, &pieces_copy, 
           SearchNode(-500, 500, currEval, turnPlayer, turnPlayer, depth, hash), &t);
    int tid = omp_get_thread_num();
    if (tid == 0){
      r = result;
    }
  }
  
  return r;
}
