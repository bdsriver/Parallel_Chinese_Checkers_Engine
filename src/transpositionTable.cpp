#include "transpositionTable.h"
#include "board.h"
#include <random>
#include <cstdint>

extern std::uint64_t pieceHashValue[PLAYER_AMOUNT][BOARD_DIM][BOARD_DIM];

extern std::uint64_t playerHashValue[PLAYER_AMOUNT];

namespace Hash{
  void initPlayerVals(std::uint64_t playerValArr[PLAYER_AMOUNT]){
    for(int i=0;i<PLAYER_AMOUNT;i++){
      playerValArr[i] = genRandom();
    }
  }

  std::uint64_t genRandom(){
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<std::uint64_t> dis;
    return dis(gen);
  }

  void initPieceVals(std::uint64_t pieceValArr[BOARD_DIM][BOARD_DIM]){
    for(int i=0;i<BOARD_DIM;i++){
      for(int j=0;j<BOARD_DIM;j++){
        if (inBounds[i][j]){
          pieceValArr[i][j] = genRandom();
        }   
      }
    } 
  }

}

TableEntry TranspositionTable::lookup(std::uint64_t hash, int search_depth){
  if(cache.contains(hash) &&  cache[hash].depth >= search_depth){
    return cache[hash];
  }
  return TableEntry(false);
}

//need to implement eviction policy
void TranspositionTable::insertEntry(std::uint64_t hash, TableEntry t){
  if(cache.contains(hash)){
    cache[hash] = t;
  }
  else {
    cache[hash] = t;
  }
  return;  
}