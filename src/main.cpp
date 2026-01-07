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
  setMovesAndJumps(2);
  Hash::initPieceVals(pieceHashValue);
  Hash::initPlayerVals(playerHashValue);
  __uint128_t occupied = pieceVectorToBoard(startPoints);
  std::vector<__uint128_t> pieces = pieceVectorToBitboards(startPoints);
  std::uint64_t hash = Hash::initHash(pieces,playersInGame);
  float startEval = posEval(pieces, 0);
  
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

  start = std::chrono::high_resolution_clock::now();
  SearchNode n(-500,500,startEval,0,0,4,hash);
  t = TranspositionTable();
  s = ignorantSearch(&occupied,&pieces[0],n,&t);
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  timeSearch = std::chrono::duration<double>(end - start).count();
  std::cout << "result: " << s.bestMove.first << ',' << s.bestMove.second <<std::endl;
  std::cout << "ignorant: "<< timeSearch << std::endl;
  
  depth = 3;
  bool ended[6] = {false,false,false,false,false,false};
  for(int i=0; i<400; i++){
    int curr = i%playersInGame;
    if (ended[curr]){
      continue;
    }
    float ev = posEval(pieces,curr);
    t = TranspositionTable();
    s = ignorantSearch(&occupied, &pieces[curr], SearchNode(-500,500,ev,curr,curr,depth,hash),&t);
    if (s.end){
      ended[curr] = true;
      continue;
    }
    makeMove(&occupied,s.bestMove);
    makeMove(&pieces[curr],s.bestMove);
    Hash::hashMove(&hash, curr, s.bestMove);
    Hash::hashTurn(&hash, curr, (curr+1)%playersInGame);
    printBitboard(pieces);
    std::cout << i << std::endl;
  }
  for (int i=0; i<playersInGame; i++){
    std::cout << posEval(pieces,i) << std::endl;
  }
  
  
  return 0;
}