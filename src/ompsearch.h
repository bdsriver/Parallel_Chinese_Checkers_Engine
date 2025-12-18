#pragma once
#include "eval.h"

SearchResult ompSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, float currEval,int depth,
  int turnPlayer);

//A transposition table made with critical sections for ompSearch
class OMPTable : public TranspositionTable{
private:

public:
  //return TableEntry where valid==false if the entry is not in the table 
  //or if it needs to be updated because search_depth > TableEntry.depth
  TableEntry lookup(std::uint64_t hash, int search_depth);

  //update entry if it already exists
  //if it doesn't exist, find one to kick out if table is full
  void insertEntry(std::uint64_t hash, TableEntry t);
};