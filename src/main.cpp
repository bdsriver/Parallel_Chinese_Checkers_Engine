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
#include <chrono>
#include <fstream>

int main(){
  srand(time(nullptr));
  setMovesAndJumps();
  Hash::initPieceVals(pieceHashValue);
  Hash::initPlayerVals(playerHashValue);
  __uint128_t occupied = boardToOccupiedBitboard(startBoard);
  std::vector<__uint128_t> pieces = pieceVectorToBitboards(startPoints);
  std::uint64_t hash = Hash::initHash(pieces,playersInGame);
  //printBoard(startBoard);
  float startEval = posEval(startPoints, 0,0);
  
  TranspositionTable t = TranspositionTable();
  int depth = 5;
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,depth,hash),&t);
  return 0;
}