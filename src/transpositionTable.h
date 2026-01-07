#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <array>
#include <mutex>
#include "board.h"

#define MAX_CACHE 50000

struct TableEntry{
  //This represents the depth of the search performed. If you searched 4 levels after, depth=4
  uint8_t depth;
  int8_t winDepth;
  uint64_t hash;
  //Best move at this position
  std::pair<int,int> bestMove;
  bool valid;
  float eval;

  TableEntry(uint8_t lookupDepth, float evaluation, std::pair<int,int> move, int _winDepth) : 
  depth(lookupDepth), eval(evaluation), bestMove(move), winDepth(_winDepth) {valid=true;};
  TableEntry(bool isValid) : valid(isValid) {};
  TableEntry() {};
};

class TranspositionTable{
protected:
  std::vector<TableEntry> table;
  size_t tableSizeMask;

public:

  TranspositionTable(size_t sizeInMB = 64) {
    size_t entries = (sizeInMB * 1024 * 1024) / sizeof(TableEntry);
    //round to power of 2 for fast modulo
    // x % 2^n == x & (2^n-1) is always true
    size_t tableSize = 1;
    while (tableSize < entries) tableSize <<= 1;
    
    table.resize(tableSize);
    tableSizeMask = tableSize - 1;
  }
  //return TableEntry where valid==false if the entry is not in the table 
  //or if it needs to be updated because search_depth > TableEntry.depth
  TableEntry lookup(std::uint64_t hash, int search_depth);

  //update entry if it already exists
  //if it doesn't exist, find one to kick out if table is full
  void insertEntry(std::uint64_t hash, TableEntry t);
};


//6 x 121
//use this for zobrist hashing
extern std::uint64_t pieceHashValue[][SPACE_AMOUNT];

extern std::uint64_t playerHashValue[];

namespace Hash{
  //must be called per color/player
  void initPieceVals(std::uint64_t pieceValArr[][SPACE_AMOUNT], int numPlayers=6);
  void initPlayerVals(std::uint64_t playerValArr[MAX_PLAYER_AMOUNT]);
  void hashMove(std::uint64_t* hash,int player, std::pair<int,int> move);
  void hashTurn(std::uint64_t* hash, int oldPlayer, int newPlayer);
  //return the hash of the initial game state with it being player 0's turn
  std::uint64_t initHash(std::vector<__uint128_t> pieces, int playerAmount, int currTurn=0);
  //Generate a random 64 bit int
  std::uint64_t genRandom();
}