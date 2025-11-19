#include <iostream>
#include "board.h"
#include "transpositionTable.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>

int main(){
  setMovesAndJumps();
  startBoard[16][4] = ' ';
  startBoard[11][5] = 3;
  std::vector<int> myPieces = initPieces(startBoard,3);
  __uint128_t occupied = boardToOccupiedBitboard(startBoard);
  std::vector<std::pair<int,int>> myMoves = generateMoves(occupied,myPieces);
  for (auto m:myMoves){
    int from = m.first;
    int to = m.second;
    std::cout << from << ',' << to << std::endl;
  }
}