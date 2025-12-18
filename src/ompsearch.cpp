#include "ompsearch.h"
#include "eval.h"
#include "board.h"
#include <omp.h>
#include <random>


SearchResult ompSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, 
  float currEval, int depth, int turnPlayer){
  
  OMPTable t = OMPTable();
  std::uint64_t hash = Hash::initHash(pieces, PLAYER_AMOUNT);
  
  int threadAmount = omp_get_max_threads();
  SearchResult r = SearchResult(false);
  
  #pragma omp parallel num_threads(threadAmount) shared(t)
  {
    // These are private (declared inside parallel region)
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



TableEntry OMPTable::lookup(std::uint64_t hash, int search_depth){
  TableEntry result = TableEntry(false);
  
  #pragma omp critical(transposition_table)
  {
    if(cache.contains(hash) && cache[hash].depth >= search_depth){
      result = cache[hash];
    }
  }  // Critical section ends here
  
  return result;
}

void OMPTable::insertEntry(std::uint64_t hash, TableEntry t){
  #pragma omp critical(transposition_table)
  {
    if(cache.contains(hash)){
      if (cache[hash].depth < t.depth){
        cache[hash] = t;
      }
    }
    else if (cacheSize < maxCacheSize){
      hashes[cacheSize] = hash;
      cacheSize++;
      cache[hash] = t;
    }
    else {
      int entries[3] = {rand()%maxCacheSize, rand()%maxCacheSize, rand()%maxCacheSize};
      int kickedOut = 0;
      int lowestDepth = cache[hashes[entries[0]]].depth;
      for (int i=1; i<3; i++){
        uint8_t d = cache[hashes[entries[i]]].depth;
        if (lowestDepth > d){
          kickedOut = i;
          lowestDepth = d;
        }
      }
      cache.erase(hashes[entries[kickedOut]]);
      hashes[entries[kickedOut]] = hash;
      cache[hash] = t;
    }
  }
  
  return;
}