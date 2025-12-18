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
  std::ofstream csvFile("timing_results.csv");
  csvFile << "Depth,Search,ThreadSearch,OMPSearch\n";

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
  int limit = 100;
  for (int depth=0; depth<=limit; depth++){
    TranspositionTable t = TranspositionTable();
    //measure this function
    auto start = std::chrono::high_resolution_clock::now();
    SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,depth,hash),&t);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double timeSearch = std::chrono::duration<double>(end - start).count();

    //measure this function
    start = std::chrono::high_resolution_clock::now();
    s = threadSearch(occupied,pieces,startEval,depth,0);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    double timeThread = std::chrono::duration<double>(end - start).count();
    
    //measure this function
    start = std::chrono::high_resolution_clock::now();
    s = ompSearch(occupied,pieces,startEval,depth,0);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    double timeOMP = std::chrono::duration<double>(end - start).count();

    csvFile << depth << "," << timeSearch << "," << timeThread << "," << timeOMP << "\n";
  }
  csvFile.close();
  return 0;
}