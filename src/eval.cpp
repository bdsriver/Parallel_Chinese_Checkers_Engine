#include "eval.h"
#include "transpositionTable.h"
#include <algorithm>


SearchResult Search(__uint128_t *board, std::vector<__uint128_t>*pieces, SearchNode node){
  int numPlayers = PLAYER_AMOUNT;
  //Did this player start the search?
  bool isStartPlayer = true ? (node.startPlayer == node.currTurn) : false;
  std::vector<std::pair<int,int>> move_spots = generateMoves(*board,(*pieces)[node.currTurn]);
  std::vector<Move> sorted_moves;

  for (std::pair<int,int> m: move_spots){
    sorted_moves.push_back(Move(m,isStartPlayer,node.currTurn));
  }

  //sort moves by best move, best moves at back
  std::sort(sorted_moves.begin(),sorted_moves.end());

  if (node.depth == 0){
    //return optimal eval
    return SearchResult(sorted_moves.back().move, node.eval + moveVal(sorted_moves.back()));
  }
  //search all possible moves and prune if possible  
  if (isStartPlayer){// max node
    //evaluate the child
    //update alpha
    // prune if alpha >= beta
    Move bestMove = Move(sorted_moves.back().move,true,node.currTurn);
    float value = -500.0;
    
    SearchNode n = SearchNode(node.alpha,node.beta,node.eval,node.startPlayer,
    (node.currTurn+1)%numPlayers,node.depth-1);
    while(!sorted_moves.empty() && n.alpha < n.beta){
      Move currMove = sorted_moves.back();
      sorted_moves.pop_back();
      makeMove(board, currMove.move);
      SearchResult r = Search(board,pieces,node);
      if (r.eval > value){
        value = r.eval;
        bestMove = currMove;
      }
      value = std::max(n.alpha, value);
      unMakeMove(board, currMove.move);
    }
    return SearchResult(bestMove.move,value);    
  }
  else{//min node
    Move bestMove = Move(sorted_moves.back().move,true,node.currTurn);
    float value = 500.0;
    SearchNode n = SearchNode(node.alpha,node.beta,node.eval,node.startPlayer,
    (node.currTurn+1)%numPlayers,node.depth-1);
    while(!sorted_moves.empty() && n.beta > n.alpha){
      Move currMove = sorted_moves.back();
      sorted_moves.pop_back();
      makeMove(board, currMove.move);
      SearchResult r = Search(board,pieces,node);
      if (r.eval < value){
        value = r.eval;
        bestMove = currMove;
      }
      value = std::min(n.alpha, value);
      unMakeMove(board, currMove.move);
    }
    return SearchResult(bestMove.move,value);
  }
  
}

float eval(std::vector<std::vector<int>> pieces, int currTurn, int startPlayer){
  int numPlayers = PLAYER_AMOUNT;
  float total = 0;
  for (int i=0;i<numPlayers; i++){
    float subtotal = 0;
    for (int p:pieces[i]){
      float val = pieceValues[i][p];
      val = (i==startPlayer) ? val : (-val) / (float)(numPlayers-1);
      subtotal += val;
    }
    total += subtotal;
    
  }
  return total;
}

float moveVal(Move m){
  float val = 0;
  val -= pieceValues[m.playerNum][m.move.first];
  val += pieceValues[m.playerNum][m.move.second];
  if (!m.isStartPlayer){
    val = val / (- (PLAYER_AMOUNT-1));
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