#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>
#include <string>

int main(){
  setMovesAndJumps();
  __uint128_t occupied = boardToOccupiedBitboard(startBoard);
  
  
  //start position eval of each move
  std::vector<std::vector<int>> pieces = startPoints;
  std::vector<std::pair<int,int>> myMoves = generateMoves(occupied,startPoints[0]);
  printBoard(startBoard);
  float startEval = eval(pieces, 0,0);
  std::cout << "Start Position Eval: " << startEval << std::endl;
  for (auto m:myMoves){
    makeMove(&occupied,m);
    float newEval = startEval - moveVal(Move(m,true,0));
    std::cout << "Move: " << m.first << ',' << m.second << " Eval: " << newEval <<std::endl;
    unMakeMove(&occupied,m);
  }
  
  

}