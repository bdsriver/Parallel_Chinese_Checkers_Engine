#include "transpositionTable.h"
#include "eval.h"
#include "board.h"

SearchResult threadSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, float currEval,int depth,
  int turnPlayer){
  TranspositionTable t = TranspositionTable();
  std::uint64_t hash = Hash::initHash(pieces,PLAYER_AMOUNT);
  //make all your threads
  //for each thread:
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,currEval,turnPlayer,turnPlayer,depth
    ,hash),&t);
  //join threads
  //use s of thread 0
  //return s of thread 0
  return s;
}