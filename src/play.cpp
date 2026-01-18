#include "play.h"
#include "board.h"
#include "transpositionTable.h"
#include "eval.h"

#include <string>
#include <random>
#include <time.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <utility>

void play(){
  std::string input;

  setMovesAndJumps(6);
  Hash::initPieceVals(pieceHashValue);
  Hash::initPlayerVals(playerHashValue);
  __uint128_t occupied = pieceVectorToBoard(startPoints);
  std::vector<__uint128_t> pieces = pieceVectorToBitboards(startPoints);
  std::uint64_t hash = Hash::initHash(pieces,playersInGame);
  bool playing[6] = {true, true, true, true, true, true};
  int currTurn = 0;
  int playerAmount = 6;
  TranspositionTable table = TranspositionTable();

  std::cout << "Ready!" << std::endl;
  while (input != "quit"){
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string command;
    std::getline(ss, command, ' ');

    std::cout << std::endl;

    if (command == "board"){
      printBitboard(pieces);
      std::cout << "Turn Player: " << currTurn << std::endl;
    }
    else if (command == "restart"){
      std::string amountString;
      std::getline(ss, amountString, ' ');
      //validate amountString as a number. default 6
      try{
        playerAmount = std::stoi(amountString);
        //can only be 2,3,4,6
        if (!(playerAmount > 1 && playerAmount < 7 && playerAmount != 5)){
          playerAmount = 6;
        }
      }
      catch(...){
        playerAmount = 6;
      }
      setMovesAndJumps(playerAmount);
      occupied = pieceVectorToBoard(startPoints);
      pieces = pieceVectorToBitboards(startPoints);
      hash = Hash::initHash(pieces,playersInGame);
      for (int i=0; i<playerAmount; i++){
        playing[i] = true;
      }
      currTurn = 0;
      table = TranspositionTable();

      std::cout << "Restarted with " << playerAmount << " players!" << std::endl;

    }

    else if (command == "go"){
      int depth;
      std::string depthString;
      std::getline(ss, depthString, ' ');
      //validate depthString as a number
      try{
        depth = std::stoi(depthString);
      }
      catch(...){
        depth = DEFAULT_DEPTH;
      }
      std::cout << "Searching at depth " << depth << ':' << std::endl;

      SearchNode n = SearchNode(-500, 500, posEval(pieces,currTurn), currTurn, currTurn, depth, hash);
      
      auto start = std::chrono::high_resolution_clock::now();
      SearchResult r = ignorantSearch(&occupied, &pieces[currTurn], n, &table);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      double timeSearch = std::chrono::duration<double>(end - start).count();

      std::cout << "Search Time: " << timeSearch << " seconds." << std::endl;
      std::cout << "Best Move: " << (int)r.bestMove.first << ',' << (int)r.bestMove.second << std::endl;

    }

    else if (command == "move"){

      //see if game is ended and get the next player to move
      int nextPlayer = playerAmount;
      for (int i=1; i<playerAmount; i++){
        int curr = (currTurn + i)%playersInGame;        
        if (playing[i]){
          nextPlayer = curr;
          break;
        }
      }
      if (nextPlayer == playerAmount){
        //check if we are the only player left
        if (playing[currTurn]){
          nextPlayer = currTurn;
        }
        else{
          std::cout << "Cannot Move: Game Ended" << std::endl;
          return;
        }
      }

      int move[2];
      std::pair<uint8_t,uint8_t> myMove;
      std::string spaceString;
      //validate depthString as a number
      try{
        std::getline(ss, spaceString, ' ');
        move[0] = std::stoi(spaceString);
        std::getline(ss, spaceString, ' ');
        move[1] = std::stoi(spaceString);
        
        myMove = {move[0],move[1]};
        if (!validMove(occupied, pieces[currTurn], myMove)){
          throw "Invalid Move";
        }

      }
      catch(...){
        std::cout << "Invalid Move";
        return;
      }

      makeMove(&occupied,myMove);
      makeMove(&pieces[currTurn],myMove);
      Hash::hashMove(&hash, currTurn, myMove);
      Hash::hashTurn(&hash, currTurn, nextPlayer);

      
      table = TranspositionTable();
      
      currTurn = nextPlayer;

      printBitboard(pieces);
    }

    else if (command == "index"){
      std::cout << "                                                            [ 0 ]                    " << std::endl;
      std::cout << "                                                       [ 1 ][ 2 ]                    " << std::endl;
      std::cout << "                                                  [ 3 ][ 4 ][ 5 ]                    " << std::endl;
      std::cout << "                                             [ 6 ][ 7 ][ 8 ][ 9 ]                    " << std::endl;
      std::cout << "                    [10 ][11 ][12 ][13 ][14 ][15 ][16 ][17 ][18 ][19 ][20 ][21 ][22 ]" << std::endl;
      std::cout << "                    [23 ][24 ][25 ][26 ][27 ][28 ][29 ][30 ][31 ][32 ][33 ][34 ]     " << std::endl;
      std::cout << "                    [35 ][36 ][37 ][38 ][39 ][40 ][41 ][42 ][43 ][44 ][45 ]          " << std::endl;
      std::cout << "                    [46 ][47 ][48 ][49 ][50 ][51 ][52 ][53 ][54 ][55 ]               " << std::endl;
      std::cout << "                    [56 ][57 ][58 ][59 ][60 ][61 ][62 ][63 ][64 ]                    " << std::endl;
      std::cout << "               [65 ][66 ][67 ][68 ][69 ][70 ][71 ][72 ][73 ][74 ]                    " << std::endl;
      std::cout << "          [75 ][76 ][77 ][78 ][79 ][80 ][81 ][82 ][83 ][84 ][85 ]                    " << std::endl;
      std::cout << "     [86 ][87 ][88 ][89 ][90 ][91 ][92 ][93 ][94 ][95 ][96 ][97 ]                    " << std::endl;
      std::cout << "[98 ][99 ][100][101][102][103][104][105][106][107][108][109][110]                    " << std::endl;
      std::cout << "                    [111][112][113][114]                                             " << std::endl;
      std::cout << "                    [115][116][117]                                                  " << std::endl;
      std::cout << "                    [118][119]                                                       " << std::endl;
      std::cout << "                    [120]                                                            " << std::endl;
    }

    else if (command == "help"){
      std::cout << "quit - stop program" << std::endl;
      std::cout << "restart <playerAmount> - restart the game, default 6 players" << std::endl;
      std::cout << "go <depth> - find the best move at a depth. default is 2" << std::endl;
      std::cout << "move <space 1> <space 2> - advance the position by moving the piece from space 1 to 2" << std::endl;
      std::cout << "board - print the ASCII board of the current position" << std::endl;
      std::cout << "index - print the ASCII board with indices labeled" << std::endl;
      std::cout << "help - command information" << std::endl;
    }

    else{
      if (input != "quit"){
        std::cout << "Invalid command. Type help for info." << std::endl;
      }
    }
    std::cout << std::endl;
  }

}