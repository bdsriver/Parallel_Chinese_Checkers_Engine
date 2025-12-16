#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>

int main(){
  setMovesAndJumps();
  __uint128_t occupied = boardToOccupiedBitboard(startBoard);
  
  
  //start position eval of each move
  std::vector<__uint128_t> pieces = pieceVectorToBitboards(startPoints);
  std::vector<std::pair<int,int>> myMoves = generateMoves(occupied,pieces[0]);
  printBoard(startBoard);
  float startEval = eval(startPoints, 0,0);
  std::cout << "Start Position Eval: " << startEval << std::endl;
  std::vector<Move> bestMoves;
  for (auto m:myMoves){
    makeMove(&occupied,m);
    float newEval = startEval - moveVal(Move(m,true,0));
    std::cout << "Move: " << m.first << ',' << m.second << " Eval: " << newEval <<std::endl;
    unMakeMove(&occupied,m);
    bestMoves.push_back(Move(m,true,0));
  }
  std::sort(bestMoves.begin(),bestMoves.end());
  for (auto m:bestMoves){
    std::cout << m.move.first << ',' << m.move.second << std::endl;
  }
  SearchResult s = Search(&occupied,&pieces,SearchNode(-500,500,startEval,0,0,0));
  std::cout << s.bestMove.first << ',' << s.bestMove.second << ':' << s.eval << std::endl;
  

}