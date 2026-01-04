#pragma once

#include <vector>
#include <utility>
#include <deque>
#include <cstdint>
#include "transpositionTable.h"

#define MAX_DEPTH 6

//Store an evaluation for the turn player and best move, and history of best moves
struct SearchResult{
  float eval;
  //best move at the position
  std::pair<int,int> bestMove;
  //history of moves with next best move at front
  bool end;

  SearchResult(std::pair<int,int> _move, float _eval) : 
  bestMove(_move), eval(_eval) {end=false;};
  SearchResult(bool _win) : end(_win) {};
};

// currturn corresponds to the player whose turn it is to move. Assume 0 goes, then 1, then 2...
// startPlayer is the player who initilialized the search
struct SearchNode
{
  float alpha, beta, eval;
  int startPlayer, currTurn;
  int depth;
  std::uint64_t hash;

  SearchNode(float _a, float _B, float _e, int _startPlayer, int _currTurn, int _depth, std::uint64_t _hash) :
  alpha(_a), beta(_B), eval(_e), startPlayer(_startPlayer), currTurn(_currTurn), depth(_depth), hash(_hash) {};
};

struct Move{
  bool isStartPlayer;
  std::pair<int,int> move;
  int playerNum;

  Move(std::pair<int,int> _move, bool _isStartPlayer, int _playerNum) :
  move(_move), isStartPlayer(_isStartPlayer), playerNum(_playerNum) {};
  Move() {};
};


//return the best move found at a position with the evaluation
// distances is the value we will use to calculate heuristics
/*  Flow (Pessimistic search):
    If depth limit reached, return optimal eval of all children nodes
    If we are the player who started the search, we are trying to maximize value (alpha)
    If we are not the player who started the search, we are trying to minimize value (Beta)
    Non-turn player wants to minimize the amount of distance all non-turn players need combined

*/
SearchResult Search(__uint128_t *board, std::vector<__uint128_t>*pieces, SearchNode node, TranspositionTable* table);

float posEval(std::vector<std::vector<int>> pieces, int currTurn, int startPlayer);

float moveVal(Move m);

bool operator<(Move m1,Move m2);