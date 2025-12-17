#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"
#include "ompsearch.h"
#include "threadSearch.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>
#include <time.h>
#include <chrono>
#include <random>
#include <string>
#include <algorithm>

int main(){
  srand(time(nullptr));
  setMovesAndJumps();
  Hash::initPieceVals(pieceHashValue);
  Hash::initPlayerVals(playerHashValue);
  __uint128_t occupied = boardToOccupiedBitboard(startBoard);
  std::vector<__uint128_t> pieces = pieceVectorToBitboards(startPoints);
  std::uint64_t hash = Hash::initHash(pieces,PLAYER_AMOUNT);
  //printBoard(startBoard);
  float startEval = posEval(startPoints, 0,0);
  
  //make a for loop iterating depth from 0 to 100 instead of using
  int depth = 50;
  
  TranspositionTable t = TranspositionTable();
  //measure this function
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,depth,hash),&t);

  //measure this function
  s = threadSearch(occupied,pieces,startEval,depth,0);

  //measure this function
  //s = ompSearch(occupied,pieces,startEval,depth,0);
  

}