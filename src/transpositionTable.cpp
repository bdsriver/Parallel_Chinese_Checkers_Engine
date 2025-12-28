#include "transpositionTable.h"
#include "board.h"
#include <random>
#include <cstdint>

std::uint64_t pieceHashValue[MAX_PLAYER_AMOUNT][SPACE_AMOUNT];

std::uint64_t playerHashValue[MAX_PLAYER_AMOUNT];

namespace Hash{
  void initPlayerVals(std::uint64_t playerValArr[MAX_PLAYER_AMOUNT]){
    for(int i=0;i<MAX_PLAYER_AMOUNT;i++){
      playerValArr[i] = genRandom();
    }
  }

  std::uint64_t genRandom(){
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<std::uint64_t> dis;
    return dis(gen);
  }

  void initPieceVals(std::uint64_t pieceValArr[][SPACE_AMOUNT], int numPlayers){
    for(int i=0;i<numPlayers;i++){
      for (int j=0; j<SPACE_AMOUNT; j++){
        pieceValArr[i][j] = genRandom();
      }
    } 
  }

  void hashMove(std::uint64_t* hash,int player, std::pair<int,int> move){
    *hash ^= pieceHashValue[player][move.first];
    *hash ^= pieceHashValue[player][move.second];
  }

  void hashTurn(std::uint64_t* hash, int oldPlayer, int newPlayer){
    *hash ^= playerHashValue[oldPlayer];
    *hash ^= playerHashValue[newPlayer];
  }

  std::uint64_t initHash(std::vector<__uint128_t> pieces, int playerAmount){
    std::uint64_t hash = 0;
    //set hash for player 0
    hash ^= playerHashValue[0];
    //set hash for every piece on the board
    for (int i=0; i<playerAmount; i++){
      uint64_t low = (uint64_t)pieces[i];
      uint64_t high = (uint64_t)(pieces[i] >> 64);
      while (low){
        int trailing_zeros = __builtin_ctzll(low);//supported by gcc
        low &= low-1; //clear that set bit
        hash ^= pieceHashValue[i][trailing_zeros];
      }
      while (high){
        int trailing_zeros = __builtin_ctzll(high) + 64;//add 64 for high address
        high &= high-1;
        hash ^= pieceHashValue[i][trailing_zeros];
      }
    }
    return hash;
  }

}

TableEntry TranspositionTable::lookup(std::uint64_t hash, int search_depth){
  size_t index = hash & tableSizeMask;
  TableEntry e = table[index];
  if(e.valid && e.hash == hash && e.depth >= search_depth){
    return e;
  }
  return TableEntry(false);
}

void TranspositionTable::insertEntry(std::uint64_t hash, TableEntry t){
  size_t index = hash & tableSizeMask;
  //use pointer for speed
  TableEntry* e = &table[index];
  if(!e->valid || e->depth <= t.depth){
    t.hash = hash;
    table[index] = t;
  }/*
  TODO: Change this to buckets
  */
  return;  
}