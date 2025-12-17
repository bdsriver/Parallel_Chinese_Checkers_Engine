#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>
#include <time.h>
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
  printBoard(startBoard);
  float startEval = posEval(startPoints, 0,0);
  std::cout << "Start Position Eval: " << startEval << std::endl;
  
  TranspositionTable t = TranspositionTable();
  float speed = -clock();
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,100,hash),&t);
  speed += clock();
  speed /= CLOCKS_PER_SEC;
  std::cout << speed << " seconds:" << std::endl; 
  std::cout << "Best Move: " << s.bestMove.first << ',' << s.bestMove.second << ':' << s.eval << std::endl;
  std::cout << "Sequence: ";
  for (auto m:s.hist){
    std::cout << m.first << ',' << m.second << ' ';
  }
  std::cout << std::endl;
  

}