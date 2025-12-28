#include "transpositionTable.h"
#include "eval.h"
#include "board.h"
#include "threadSearch.h"
#include <thread>
#include <vector>

SearchResult threadSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, 
  float currEval, int depth, int turnPlayer){
  
  TranspositionTable t = TranspositionTable();
  std::uint64_t hash = Hash::initHash(pieces, playersInGame);
  
  int threadAmount = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  
  std::vector<__uint128_t> occupied_copies(threadAmount, occupied);
  std::vector<std::vector<__uint128_t>> pieces_copies(threadAmount, pieces);
  
  for (int i = 0; i < threadAmount; i++){
    threads.push_back(std::thread(Search, &occupied_copies[i],&pieces_copies[i],
      SearchNode(-500, 500, currEval, turnPlayer, turnPlayer, depth, hash),&t));
  }
  
  for (auto& thread : threads){
    thread.join();
  }
  
  TableEntry e = t.lookup(hash, 0);
  return SearchResult(e.bestMove, e.eval, std::deque<std::pair<int,int>>({e.bestMove}));
}