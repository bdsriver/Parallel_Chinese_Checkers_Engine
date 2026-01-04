#include "eval.h"
#include "transpositionTable.h"
#include <algorithm>
#include <vector>
#include <iostream>


SearchResult Search(__uint128_t *board, std::vector<__uint128_t>*pieces, SearchNode node, TranspositionTable* table){
  //check table
  TableEntry t = table->lookup(node.hash,node.depth);
  if (t.valid){
    return SearchResult(t.bestMove,t.eval);
  }

  //Did this player start the search?
  bool isStartPlayer = true ? (node.startPlayer == node.currTurn) : false;
  std::pair<int,int> move_spots[MAX_MOVES];
  int moveAmount = generateMoves(move_spots,*board,(*pieces)[node.currTurn],node.currTurn);
  if (moveAmount == -1){
    //this means this is a winning position
    return SearchResult(true);
  }
  Move sorted_moves[MAX_MOVES];

  for (int i=0; i<moveAmount; i++){
    sorted_moves[i] = Move(move_spots[i],isStartPlayer,node.currTurn);
  }

  //sort moves by best move, best moves at back
  std::sort(sorted_moves,sorted_moves+moveAmount);

  if (node.depth == 0){
    float e = node.eval + moveVal(sorted_moves[moveAmount-1]);
    table->insertEntry(node.hash, TableEntry(node.depth,e,sorted_moves[moveAmount-1].move));
    //return optimal eval
    return SearchResult(sorted_moves[moveAmount-1].move, e);
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
        return SearchResult(currMove.move,500);
      }
      if (r.eval > value){
        value = r.eval;
        bestMove = currMove;
      }
      n.alpha = std::max(n.alpha, value);
      
      n.eval -= m_val;
    }
    table ->insertEntry(node.hash,TableEntry(node.depth,value,bestMove.move));
    return SearchResult(bestMove.move,value);    
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
        return SearchResult(currMove.move,500);
      }
      if (r.eval < value){
        value = r.eval;
        bestMove = currMove;
      }
      n.beta = std::min(n.beta, value);
      n.eval -= m_val;
    }
    table ->insertEntry(node.hash,TableEntry(node.depth,value,bestMove.move));
    return SearchResult(bestMove.move,value);
  }
  
}

float posEval(std::vector<std::vector<int>> pieces, int currTurn, int startPlayer){
  float total = 0;
  for (int i=0;i<playersInGame; i++){
    float subtotal = 0;
    for (int p:pieces[i]){
      float val = pieceValues[i][p];
      val = (i==startPlayer) ? val : (-val) / (float)(playersInGame-1);
      subtotal += val;
    }
    total += subtotal;
    
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

bool operator<(Move m1,Move m2){
  float v1, v2;
  v1 = pieceValues[m1.playerNum][m1.move.second] - pieceValues[m1.playerNum][m1.move.first];
  v2 = pieceValues[m2.playerNum][m2.move.second] - pieceValues[m2.playerNum][m2.move.first];
  //This makes it so that each player's best move is at the end
  return v1 > v2;
}