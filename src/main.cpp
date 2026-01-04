#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"
#include "ompsearch.h"
#include "threadSearch.h"
#include <vector>
#include <random>
#include <string>
#include <chrono>

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
  int depth = 6;

  //SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,depth,hash),&t);

  
  auto start = std::chrono::high_resolution_clock::now();
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,depth,hash),&t);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  double timeSearch = std::chrono::duration<double>(end - start).count();
  std::cout << "result: " << s.bestMove.first << ',' << s.bestMove.second << ": " << s.eval << std::endl;
  std::cout << "sequential: "<< timeSearch << std::endl;
  /*
  start = std::chrono::high_resolution_clock::now();
  s = threadSearch(occupied,pieces,startEval,depth,0);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  timeSearch = std::chrono::duration<double>(end - start).count();
  std::cout << "threads: "<< timeSearch << std::endl;


  start = std::chrono::high_resolution_clock::now();
  s = ompSearch(occupied,pieces,startEval,depth,0);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  timeSearch = std::chrono::duration<double>(end - start).count();
  std::cout << "omp: "<< timeSearch << std::endl;
  */

  return 0;
}