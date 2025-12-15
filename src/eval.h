#pragma once

#include <vector>
#include <utility>

#define MAX_DEPTH 6

//Store an evaluation for the turn player and best move
struct SearchResult{
  float eval;
  //best move at the position
  std::pair<int,int> bestMove;

  SearchResult(std::pair<int,int> _move, float _eval) : bestMove(_move), eval(_eval) {};
};

// currturn corresponds to the player whose turn it is to move. Assume 0 goes, then 1, then 2...
// startPlayer is the player who initilialized the search
struct SearchNode
{
  float alpha, beta, eval;
  int startPlayer, currTurn;
  int depth;

  SearchNode(float _a, float _B, float _e, int _startPlayer, int _currTurn, int _depth) :
  alpha(_a), beta(_B), startPlayer(_startPlayer), currTurn(_currTurn), depth(_depth) {};
};

struct Move{
  bool isStartPlayer;
  std::pair<int,int> move;
  int playerNum;

  Move(std::pair<int,int> _move, bool _isStartPlayer, int _playerNum) :
  move(_move), isStartPlayer(_isStartPlayer), playerNum(_playerNum) {};
};


//return the best move found at a position with the evaluation
// distances is the value we will use to calculate heuristics
/*  Flow (Pessimistic search):
    If depth limit reached, return optimal eval of all children nodes
    If we are the player who started the search, we are trying to maximize value (alpha)
    If we are not the player who started the search, we are trying to minimize value (Beta)
    Non-turn player wants to minimize the amount of distance all non-turn players need combined

*/
SearchResult Search(__uint128_t *board, std::vector<std::vector<int>> *pieces, SearchNode *node);

float eval(std::vector<std::vector<int>> pieces, int currTurn, int startPlayer);

float moveVal(Move m);