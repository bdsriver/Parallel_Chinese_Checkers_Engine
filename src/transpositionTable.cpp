#include "transpositionTable.h"
#include "board.h"
#include <random>
#include <cstdint>

std::uint64_t pieceHashValue[PLAYER_AMOUNT][SPACE_AMOUNT];

std::uint64_t playerHashValue[PLAYER_AMOUNT];

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
  if(cache.contains(hash) &&  cache[hash].depth >= search_depth){
    return cache[hash];
  }
  return TableEntry(false);
}

void TranspositionTable::insertEntry(std::uint64_t hash, TableEntry t){
  if(cache.contains(hash)){
    if (cache[hash].depth < t.depth){
      cache[hash] = t;
    }
  }
  else if (cacheSize < maxCacheSize){
    hashes[cacheSize] = hash;
    cacheSize++;
    cache[hash] = t;
  }
  else {
    //pick 3 random entries and kick one out
    int entries[3] = {rand()%maxCacheSize, rand()%maxCacheSize, rand()%maxCacheSize};
    int kickedOut = 0;
    int lowestDepth = cache[hashes[entries[0]]].depth;
    for (int i=1; i<3; i++){
      uint8_t d = cache[hashes[entries[i]]].depth;
      if (lowestDepth > d){
        kickedOut = i;
        lowestDepth = d;
      }
    }
    cache.erase(hashes[entries[kickedOut]]);
    hashes[entries[kickedOut]] = hash;
    cache[hash] = t;
  }
  return;  
}