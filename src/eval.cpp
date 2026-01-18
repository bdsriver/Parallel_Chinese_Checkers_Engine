#include "eval.h"
#include "transpositionTable.h"

#include <vector>
#include <utility>
#include <cstdint>


/*
SearchResult Search(__uint128_t *board, std::vector<__uint128_t>*pieces, SearchNode node, TranspositionTable* table){
  //check table
  TableEntry t = table->lookup(node.hash,node.depth);
  if (t.valid){
    return SearchResult(t.bestMove,t.eval, t.winDepth);
  }

  //Did this player start the search?
  bool isStartPlayer = true ? (node.startPlayer == node.currTurn) : false;
  std::pair<uint8_t,uint8_t> move_spots[MAX_MOVES];
  int moveAmount = generateMoves(move_spots,*board,(*pieces)[node.currTurn],node.currTurn);
  if (moveAmount == -1){
    //this means this is a winning position
    return SearchResult(true, node.depth);
  }
  Move sorted_moves[MAX_MOVES];

  for (int i=0; i<moveAmount; i++){
    sorted_moves[i] = Move(move_spots[i],isStartPlayer,node.currTurn);
  }

  //sort moves by best move, best moves at back
  std::sort(sorted_moves,sorted_moves+moveAmount);

  if (node.depth == 0){
    float e = node.eval + moveVal(sorted_moves[moveAmount-1]);
    table->insertEntry(node.hash, TableEntry(node.depth,e,sorted_moves[moveAmount-1].move,-1));
    //return optimal eval
    return SearchResult(sorted_moves[moveAmount-1].move, e,-1);
  }
  //search all possible moves and prune if possible  
  int searchTurn = node.currTurn;
  if (isStartPlayer){// max node
    //evaluate the child
    //update alpha
    // prune if alpha >= beta
    int back = moveAmount -1;
    Move bestMove = Move(sorted_moves[back].move,true,searchTurn);
    float value = -500.0;
    
    SearchNode n = SearchNode(node.alpha,node.beta,node.eval,node.startPlayer,
    (node.currTurn+1)%playersInGame,node.depth-1,node.hash);
    Hash::hashTurn(&n.hash, node.currTurn,n.currTurn);
    while(back >= 0 && n.alpha < n.beta){
      Move currMove = sorted_moves[back];
      back--;
      makeMove(board, currMove.move);
      makeMove(&(*pieces)[searchTurn], currMove.move);
      Hash::hashMove(&n.hash, searchTurn, currMove.move);
      float m_val = moveVal(currMove);
      n.eval += m_val;
      SearchResult r = Search(board,pieces,n,table);
      unMakeMove(board, currMove.move);
      unMakeMove(&(*pieces)[searchTurn], currMove.move);
      Hash::hashMove(&n.hash, searchTurn, currMove.move);
      if (r.end){
        return SearchResult(currMove.move,500,r.winDepth);
      }
      if (r.eval > value){
        value = r.eval;
        bestMove = currMove;
      }
      n.alpha = std::max(n.alpha, value);
      
      n.eval -= m_val;
    }
    table ->insertEntry(node.hash,TableEntry(node.depth,value,bestMove.move,-1));
    return SearchResult(bestMove.move,value,-1);    
  }
  else{//min node
    int back = moveAmount-1;
    Move bestMove = Move(sorted_moves[back].move,true,searchTurn);
    float value = 500.0;
    SearchNode n = SearchNode(node.alpha,node.beta,node.eval,node.startPlayer,
    (node.currTurn+1)%playersInGame,node.depth-1,node.hash);
    Hash::hashTurn(&n.hash, node.currTurn,n.currTurn);
    while(back >= 0 && n.beta > n.alpha){
      Move currMove = sorted_moves[back];
      back--;
      makeMove(board, currMove.move);
      makeMove(&(*pieces)[searchTurn], currMove.move);
      Hash::hashMove(&n.hash, searchTurn, currMove.move);
      float m_val = moveVal(currMove);
      n.eval += m_val;
      SearchResult r = Search(board,pieces,n,table);
      unMakeMove(board, currMove.move);
      unMakeMove(&(*pieces)[searchTurn], currMove.move);
      Hash::hashMove(&n.hash, searchTurn, currMove.move);
      if (r.end){
        return SearchResult(currMove.move,500,r.winDepth);
      }
      if (r.eval < value){
        value = r.eval;
        bestMove = currMove;
      }
      n.beta = std::min(n.beta, value);
      n.eval -= m_val;
    }
    table ->insertEntry(node.hash,TableEntry(node.depth,value,bestMove.move,-1));
    return SearchResult(bestMove.move,value,-1);
  }
  
}*/

float posEval(std::vector<__uint128_t> pieces, int currTurn){
  float total = 0;
  for (int i=0;i<playersInGame; i++){
    uint64_t low = (uint64_t)pieces[i];
    uint64_t high = (uint64_t)(pieces[i] >> 64);
    while (low){
      int trailing_zeros = __builtin_ctzll(low);//supported by gcc
      low &= low-1; //clear that set bit
      float val = pieceValues[i][trailing_zeros];
      val = (i==currTurn) ? -val : (val) / (float)(playersInGame-1);
      total += val;
    }
    while (high){
      int trailing_zeros = __builtin_ctzll(high)+64;//add 64 for high address
      high &= high-1;
      float val = pieceValues[i][trailing_zeros];
      val = (i==currTurn) ? -val : (val) / (float)(playersInGame-1);
      total += val;
    }
  }
  return total;
}

float moveVal(Move m){
  float val = 0;
  val += pieceValues[m.playerNum][m.move.first];
  val -= pieceValues[m.playerNum][m.move.second];
  if (!m.isStartPlayer){
    val = val / (- (playersInGame-1));
  }
  return val;
}

float moveVal(std::pair<uint8_t,uint8_t> move, int playerNum, bool isStartPlayer){
  float val = 0;
  val += pieceValues[playerNum][move.first];
  val -= pieceValues[playerNum][move.second];
  if (!isStartPlayer){
    val = val / (- (playersInGame-1));
  }
  return val;
}

bool operator<(Move m1,Move m2){
  float v1, v2;
  v1 = pieceValues[m1.playerNum][m1.move.second] - pieceValues[m1.playerNum][m1.move.first];
  v2 = pieceValues[m2.playerNum][m2.move.second] - pieceValues[m2.playerNum][m2.move.first];
  //This makes it so that each player's best move is at the end
  return v1 > v2;
}

SearchResult ignorantSearch(__uint128_t *board, __uint128_t * pieces, SearchNode node,
 TranspositionTable* table)
{
  //check table
  TableEntry t = table->lookup(node.hash,node.depth);
  if (t.valid){
    return SearchResult(t.bestMove,t.eval, t.winDepth, t.bestDepth);
  }

  std::pair<uint8_t,uint8_t> move_spots[MAX_MOVES];
  int moveAmount = generateMoves(move_spots,*board,*pieces,node.currTurn);
  if (moveAmount == -1){
    //this means this is a winning position
    return SearchResult(true, node.depth);
  }


  if (node.depth == 0){
    //find best value move out of current options
    std::pair<uint8_t,uint8_t> bestMove = move_spots[0];
    float bestVal = moveVal(bestMove, node.currTurn, true);
    for(int i=1; i<moveAmount; i++){
      float compVal = moveVal(move_spots[i],node.currTurn,true);
      if (bestVal < compVal){
        bestMove = move_spots[i];
        bestVal = compVal;
      }
    }

    float e = node.eval + bestVal;
    table->insertEntry(node.hash, TableEntry(node.depth,e,bestMove, -1, node.depth));
    //return optimal eval
    return SearchResult(bestMove, e, -1, node.depth);
  }

  //find best option among all possible moves
  int back = moveAmount-1;
  std::pair<uint8_t,uint8_t> bestMove;
  float bestEval = -500;
  int winDepth = -1;
  node.depth -= 1;
  uint8_t bestDepth = 0;
  while(back>=0){
    std::pair<uint8_t,uint8_t> currMove = move_spots[back];
    back--;

    makeMove(board,currMove);
    makeMove(pieces,currMove);
    Hash::hashMove(&(node.hash), node.currTurn, currMove);
    float currMoveVal = moveVal(currMove,node.currTurn, true);
    node.eval += currMoveVal;

    SearchResult r = ignorantSearch(board, pieces, node, table);

    node.eval -= currMoveVal;
    Hash::hashMove(&(node.hash), node.currTurn, currMove);
    unMakeMove(pieces, currMove);
    unMakeMove(board, currMove);

    //check if we reach a winning position from here
    if (r.end){
      table->insertEntry(node.hash, TableEntry(node.depth+1,500,currMove,r.winDepth, node.depth+1));
      return SearchResult(currMove, 500, r.winDepth, node.depth+1);
    }

    if (r.winDepth > winDepth){
      bestEval = r.eval;
      bestMove = currMove;
      winDepth = r.winDepth;
      bestDepth = r.bestEvalDepth;
      continue;
    }
    
    //if we have equal value take the result with better intermediate value
    if (r.eval == bestEval && r.bestEvalDepth > bestDepth){
      bestEval = r.eval;
      bestMove = currMove;
      winDepth = r.winDepth;
      bestDepth = r.bestEvalDepth;
      continue;
    }

    if (r.eval > bestEval){
      bestEval = r.eval;
      bestMove = currMove;
      winDepth = r.winDepth;
      bestDepth = r.bestEvalDepth;
    }
  }
  if (moveVal(bestMove,node.currTurn,true) > 0){
    bestDepth = node.depth+1;
  }
  table->insertEntry(node.hash, TableEntry(node.depth+1,bestEval,bestMove,winDepth, bestDepth));
  return SearchResult(bestMove,bestEval, winDepth, bestDepth);
}