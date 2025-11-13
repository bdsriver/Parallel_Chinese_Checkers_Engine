#pragma once

#include <cstdint>
#include <unordered_map>
#include "board.h"

#define MAX_CACHE 5000

struct TableEntry{
  //This represents the depth of the search performed. If you searched 4 levels after, depth=4
  int depth;
  //Best move at this position. Hole (x1,y1) --> Hole (x2,y2)
  int x1, y1, x2, y2;
  double eval;
  bool valid;

  TableEntry(int lookupDepth, double evaluation, int _x1, int _y1, int _x2, int _y2) : 
  depth(lookupDepth), eval(evaluation), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {valid=true;};
  TableEntry(bool isValid) : valid(isValid) {};
  TableEntry() {};
};

class TranspositionTable{
private:
  std::unordered_map<std::uint64_t, TableEntry> cache;
  int cacheSize;
  int maxCacheSize;

public:
  //return TableEntry where valid==false if the entry is not in the table 
  //or if it needs to be updated because search_depth > TableEntry.depth
  TableEntry lookup(std::uint64_t hash, int search_depth);

  //update entry if it already exists
  //if it doesn't exist, find one to kick out if table is full
  void insertEntry(std::uint64_t hash, TableEntry t);

  TranspositionTable() {maxCacheSize=MAX_CACHE; cacheSize=0; }
  TranspositionTable(int max_cache_size) : maxCacheSize(max_cache_size) {cacheSize=0;}
};


//6 x 17 x 17
//use this for zobrist hashing
extern std::uint64_t pieceHashValue[][BOARD_DIM][BOARD_DIM];

extern std::uint64_t playerHashValue[];

namespace Hash{
  //must be called per color/player
  void initPieceVals(int pieceValArr[BOARD_DIM][BOARD_DIM]);
  void initPlayerVals(int playerValArr[PLAYER_AMOUNT]);
  //Generate a random 64 bit int
  std::uint64_t genRandom();
}