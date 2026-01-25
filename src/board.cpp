#include "board.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <deque>
#include <algorithm>

int playersInGame;

//must call setMovesAndJumps() to initiallize this
//key: position(0-121) value: bitboard (set to 1 at valid move location)
__uint128_t moves[SPACE_AMOUNT];

//must call setMovesAndJumps() to initiallize this
//key: position(0-121) value: bitboard (set to 1 at valid jump location)
__uint128_t jumps[SPACE_AMOUNT];
std::vector<std::unordered_map<int,int>> halfJumps;
//initiallized in setMovesAndJumps()
std::vector<std::vector<float>> pieceValues;
//changed during runtime
std::vector<std::vector<int>> startPoints;
//changed during runtime
std::vector<std::vector<int>> endZones;


//used to construct startPoints and endZones, not currently in header
std::vector<std::vector<int>> defaultStartPoints = {
  {6,7,8,9,0,1,2,3,4,5},
  {19,32,44,55,20,21,22,33,34,45},
  {74,84,95,107,85,96,97,108,109,110},
  {111,112,113,114,115,116,117,118,119,120},
  {65,76,88,101,75,86,87,98,99,100},
  {13,25,36,46,10,11,12,23,24,35}
};

bool inBounds[BOARD_DIM][BOARD_DIM] = {
  {false,false,false,false,false,false,false,false,false,false,false,false,true ,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,false,true ,true ,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,false,true ,true ,true ,false,false,false,false},
  {false,false,false,false,false,false,false,false,false,true ,true ,true ,true ,false,false,false,false},
  {false,false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true },
  {false,false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false},
  {false,false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false},
  {false,false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false},
  {false,false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false,false},
  {false,false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false,false},
  {false,false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false,false},
  {false,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false,false},
  {true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,false,false,false,false},
  {false,false,false,false,true ,true ,true ,true ,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,true ,true ,true ,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,true ,true ,false,false,false,false,false,false,false,false,false,false,false},
  {false,false,false,false,true ,false,false,false,false,false,false,false,false,false,false,false,false}
};

//A mapping of a single number to the location in 2D array int -> (x,y)
std::unordered_map <int, std::pair<uint8_t,uint8_t>> bitToIndices = {
  {0, {12,0}}, {1, {11,1}}, {2, {12,1}}, {3, {10,2}}, {4, {11,2}}, {5, {12,2}},        
  {6, {9,3}}, {7, {10,3}}, {8, {11,3}}, {9, {12,3}}, {10, {4,4}}, {11, {5,4}},        
  {12, {6,4}}, {13, {7,4}}, {14, {8,4}}, {15, {9,4}}, {16, {10,4}}, {17, {11,4}},      
  {18, {12,4}}, {19, {13,4}}, {20, {14,4}}, {21, {15,4}}, {22, {16,4}}, {23, {4,5}},      
  {24, {5,5}}, {25, {6,5}}, {26, {7,5}}, {27, {8,5}}, {28, {9,5}}, {29, {10,5}},       
  {30, {11,5}}, {31, {12,5}}, {32, {13,5}}, {33, {14,5}}, {34, {15,5}}, {35, {4,6}},      
  {36, {5,6}}, {37, {6,6}}, {38, {7,6}}, {39, {8,6}}, {40, {9,6}}, {41, {10,6}},       
  {42, {11,6}}, {43, {12,6}}, {44, {13,6}}, {45, {14,6}}, {46, {4,7}}, {47, {5,7}},       
  {48, {6,7}}, {49, {7,7}}, {50, {8,7}}, {51, {9,7}}, {52, {10,7}}, {53, {11,7}},      
  {54, {12,7}}, {55, {13,7}}, {56, {4,8}}, {57, {5,8}}, {58, {6,8}}, {59, {7,8}},        
  {60, {8,8}}, {61, {9,8}}, {62, {10,8}}, {63, {11,8}}, {64, {12,8}}, {65, {3,9}},      
  {66, {4,9}}, {67, {5,9}}, {68, {6,9}}, {69, {7,9}}, {70, {8,9}}, {71, {9,9}},        
  {72, {10,9}}, {73, {11,9}}, {74, {12,9}}, {75, {2,10}}, {76, {3,10}}, {77, {4,10}},     
  {78, {5,10}}, {79, {6,10}}, {80, {7,10}}, {81, {8,10}}, {82, {9,10}}, {83, {10,10}},    
  {84, {11,10}}, {85, {12,10}}, {86, {1,11}}, {87, {2,11}}, {88, {3,11}}, {89, {4,11}},     
  {90, {5,11}}, {91, {6,11}}, {92, {7,11}}, {93, {8,11}}, {94, {9,11}}, {95, {10,11}},    
  {96, {11,11}}, {97, {12,11}}, {98, {0,12}}, {99, {1,12}}, {100, {2,12}}, {101, {3,12}},   
  {102, {4,12}}, {103, {5,12}}, {104, {6,12}}, {105, {7,12}}, {106, {8,12}}, {107, {9,12}},  
  {108, {10,12}}, {109, {11,12}}, {110, {12,12}}, {111, {4,13}}, {112, {5,13}}, {113, {6,13}},  
  {114, {7,13}}, {115, {4,14}}, {116, {5,14}}, {117, {6,14}}, {118, {4,15}}, {119, {5,15}}, {120, {4,16}}  
};

//refer to diagram for mapping explanation
void setMovesAndJumps(int playerAmount){
  playersInGame = playerAmount;
  auto isInBounds = [](int x, int y) -> bool {
    return x>=0 && y>=0 && x<BOARD_DIM && y<BOARD_DIM && inBounds[y][x];
  };
  auto setBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n | ((__uint128_t)1 << bit));
  };

  int moveDirs[6][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,-1},{-1,1}};
  int jumpDirs[6][2] = { {2,0},{-2,0},{0,2},{0,-2},{2,-2},{-2,2}};

  //create backwards map for this function
  int indicesToBit[BOARD_DIM][BOARD_DIM];
  for (int i=0; i<SPACE_AMOUNT; i++){
    std::pair<uint8_t, uint8_t> p = bitToIndices[i];
    indicesToBit[p.second][p.first] = i;
  }
  
  halfJumps.resize(SPACE_AMOUNT);
  //for every space, map out the moves and jumps you can make from there
  for (int i=0;i<SPACE_AMOUNT;i++){
    std::pair<uint8_t, uint8_t> p = bitToIndices[i];
    int x = p.first;
    int y = p.second;
    std::vector<int> moves_possible, jumps_possible;
    for (int j=0; j<6; j++){
      int newX = x+moveDirs[j][0];
      int newY = y+moveDirs[j][1];
      if (isInBounds(newX, newY)){
        moves_possible.push_back(indicesToBit[newY][newX]);
      }

      newX = x+jumpDirs[j][0];
      newY = y+jumpDirs[j][1];
      int hx = x+(jumpDirs[j][0]/2);
      int hy = y+(jumpDirs[j][1]/2);
      if (isInBounds(newX, newY)){
        int jumpLanding = indicesToBit[newY][newX];
        jumps_possible.push_back(jumpLanding);
        halfJumps[i][jumpLanding] = indicesToBit[hy][hx];
      }
    }
    //now that jumps and moves have the squares you can go to from square i,
    //we can put them into the bitboard
    __uint128_t bitboard = 0;
    for (auto m:moves_possible){
      bitboard = setBit(bitboard,m);
    }
    moves[i] = bitboard;

    __uint128_t bitboard2 = 0;
    for (auto j:jumps_possible){
      bitboard2 = setBit(bitboard2,j);
    }
    jumps[i] = bitboard2;

  }

  if (playersInGame == 2){
    std::vector<std::vector<int>> temp = defaultStartPoints;
    startPoints = {temp[0],temp[3]};
    endZones = {temp[3],temp[0]};
  }
  else if (playersInGame == 3){
    std::vector<std::vector<int>> temp = defaultStartPoints;
    startPoints = {temp[0],temp[2],temp[4]};
    endZones = {temp[3],temp[5],temp[1]};
  }
  else if (playersInGame == 4){
    std::vector<std::vector<int>> temp = defaultStartPoints;
    startPoints = {temp[0],temp[1],temp[3],temp[4]};
    endZones = {temp[3],temp[4],temp[0],temp[1]};
  }
  else //if (playersInGame == 6)
  {
    std::vector<std::vector<int>> temp = defaultStartPoints;
    startPoints = defaultStartPoints;
    endZones = {temp[3],temp[4],temp[5],temp[0],temp[1],temp[2]};
  }


  //use djikstra's to set piece table by measuring distance from end
  // we start with all spaces unexplored and set to max value except the starting spaces
  // we can use a normal queue instead of priority queue because there is 1 unit between each adjacent space
  std::vector<std::deque<int>> queue;
  std::vector<__uint128_t> explored;
  pieceValues = {};
  for (int i=0 ; i<playersInGame; i++){
    pieceValues.push_back({});
    for (int j=0; j<SPACE_AMOUNT; j++){
      pieceValues[i].push_back(248);
    }
    explored.push_back(0);
    queue.push_back({});    
    for (int j=0; j<10; j++){
      explored[i] = setBit(explored[i], endZones[i][j]);
      if (j<4){
        queue[i].push_back(endZones[i][j]);
        pieceValues[i][endZones[i][j]] = 0;
      }
      else{
        pieceValues[i][endZones[i][j]] = -1;
      }
    }
  }
  //pop front of the queue, get all moves from that space that haven't been explored yet,
  //add 
  auto djikstra = [&](int player) -> void {
    while (queue[player].size() > 0){
      int u = queue[player].front();
      queue[player].pop_front();
      
      std::vector<int> edges = {};

      //split 128 bit int into two 64 bit ints
      __uint128_t pieceMoves = (~explored[player]) & moves[u];
      uint64_t low = (uint64_t)pieceMoves;
      uint64_t high = (uint64_t)(pieceMoves >> 64);
      while (low){
        int trailing_zeros = __builtin_ctzll(low);//supported by gcc
        low &= low-1; //clear that set bit
        edges.push_back(trailing_zeros);
      }
      while (high){
        int trailing_zeros = __builtin_ctzll(high)+64;//add 64 for high address
        high &= high-1;
        edges.push_back(trailing_zeros);
      }
      //add all unexplored moves from here to queue 
      for (int v:edges){
        if(pieceValues[player][u]+1 < pieceValues[player][v]){
          pieceValues[player][v] = pieceValues[player][u] + 1;
          queue[player].push_back(v);
        }
      }
    }
  };//end djikstra definition
  
  for (int i=0; i<playersInGame; i++){
    djikstra(i);
  }
  for (int i=0; i<playersInGame; i++){
    std::sort(endZones[i].begin(), endZones[i].end());
  }

}

void printBoard(char board[][17]) {
  for (int i = 0; i < 17; i++) {
    for (int j = 0; j < 17; j++) {
      if (inBounds[i][j]) {
        if (board[i][j] == '+') { 
            printf("[+]");
        } 
        else if (board[i][j] >= 0 && board[i][j] <= 9) {
            printf("[%d]", board[i][j]);
        } 
        else {
            printf("[ ]");
        }
      } else {
          printf("   ");
      }
    }
    printf("\n");
  }
}

int generateMoves(std::pair<uint8_t,uint8_t> moveArr[MAX_MOVES],__uint128_t occupied, __uint128_t pieces, int player){
  int moveAmount = 0;

  auto setBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n | ((__uint128_t)1 << bit));
  };
  auto unsetBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n & (~((__uint128_t)1 << bit)));
  };
  auto bitIsSet = [](__uint128_t n, int bit) -> bool {
    return (n & ((__uint128_t)1 << bit) ) ;
  };
  
  //Generate one-space moves
  __uint128_t temp = pieces;
  int pieceLocations[PLAYER_PIECE_AMOUNT];
  for (int i=0; i<PLAYER_PIECE_AMOUNT; i++){
    int startPos = 0;
    if((uint64_t)temp){
      startPos = __builtin_ctzll((uint64_t)temp);
    }
    else{
      startPos = __builtin_ctzll((uint64_t)(temp >> 64)) + 64;
    }
    pieceLocations[i] = startPos;
    temp &= temp -1;

    __uint128_t pieceMoves = moves[startPos] & (~occupied);
    //find set bits to get possible piece squares
    //split 128 bit uint into 2 64 bit uints
    uint64_t low = (uint64_t)pieceMoves;
    uint64_t high = (uint64_t)(pieceMoves >> 64);
    while (low){
      int trailing_zeros = __builtin_ctzll(low);//supported by gcc
      low &= low-1; //clear that set bit
      moveArr[moveAmount] = {startPos,trailing_zeros};
      moveAmount++;
    }
    while (high){
      int trailing_zeros = __builtin_ctzll(high) + 64;//add 64 for high register
      high &= high-1;
      moveArr[moveAmount] = {startPos,trailing_zeros};
      moveAmount++;
    }
  }
  //if all pieces are in the end zone, you win, no moves to make
  bool inEndZone = true;
  for (int i=0; i<PLAYER_PIECE_AMOUNT; i++){
    if (pieceLocations[i] != endZones[player][i]){
      inEndZone = false;
      break;
    }
  }
  if(inEndZone){
    return -1;
  }

  //Generate jumps with DFS
  temp = pieces;
  for (int i=0; i<PLAYER_PIECE_AMOUNT; i++){
    //start with a point to jump from being your current spot and add new points
    int startPos;
    if((uint64_t)temp){
      startPos = __builtin_ctzll((uint64_t)temp);
    }
    else{
      startPos = __builtin_ctzll((uint64_t)(temp >> 64)) + 64;
    }
    temp &= temp-1;
    int jumpPoints[25];
    jumpPoints[0] = startPos;
    int back = 0;
    // "hashmap" to check if we already jumped to a spot
    __uint128_t jumpedTo = setBit(0,startPos); 


    while (back >= 0) {
      int startSpace = jumpPoints[back];
      back--;

      //add all possible jumps to stack
      //first compute jumps and then check if their intermediate space is occupied to verify
      __uint128_t pieceJumps = jumps[startSpace] & (~occupied);
      uint64_t low = (uint64_t)pieceJumps;
      uint64_t high = (uint64_t)(pieceJumps >> 64);
      while (low){
        int trailing_zeros = __builtin_ctzll(low);
        low &= low-1;
        if (bitIsSet(occupied,halfJumps[startSpace][trailing_zeros])  && bitIsSet(~jumpedTo,trailing_zeros)){
          moveArr[moveAmount] = {startPos,trailing_zeros};
          moveAmount++;
          back++;
          jumpPoints[back] = trailing_zeros;
          //set the hash
          jumpedTo = setBit(jumpedTo, trailing_zeros);
        }        
      }
      while (high){
        int trailing_zeros = __builtin_ctzll(high) + 64;//add 64 for high address
        high &= high-1;
        //check that there is a piece to jump over && we have not visited the landing space on our search
        if (bitIsSet(occupied,halfJumps[startSpace][trailing_zeros]) && bitIsSet(~jumpedTo,trailing_zeros)){
          moveArr[moveAmount] = {startPos,trailing_zeros};
          moveAmount++;
          back++;
          jumpPoints[back] = trailing_zeros;
          jumpedTo = setBit(jumpedTo,trailing_zeros);
        }
      }
    }//end while loop
  }//end iteration i


  return moveAmount;
}

void makeMove(__uint128_t *occupied, std::pair<uint8_t,uint8_t> move){
  //set the new space bit
  *occupied = (*occupied) | ((__uint128_t)1 << move.second);
  //unset the old space bit
  *occupied = (*occupied) & ~(((__uint128_t)1 << move.first));
}

void unMakeMove(__uint128_t *occupied, std::pair<uint8_t,uint8_t> move){
  //set the old space bit
  *occupied = (*occupied) | ((__uint128_t)1 << move.first);
  //unset the new space bit
  *occupied = (*occupied) & ~((__uint128_t)1 << move.second);
}

std::vector<__uint128_t> pieceVectorToBitboards(std::vector<std::vector<int>> pieces){
  auto setBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n | ((__uint128_t)1 << bit));
  };
  std::vector<__uint128_t> newPieces;
  for (std::vector<int> v:pieces){
    __uint128_t bitboard = 0;
    for(int p:v){
      bitboard = setBit(bitboard,p);
    }
    newPieces.push_back(bitboard);
  }
  return newPieces;
}

__uint128_t pieceVectorToBoard(std::vector<std::vector<int>> pieces){
  std::vector<__uint128_t> bitboards = pieceVectorToBitboards(pieces);
  __uint128_t board = 0;
  for (int i=0; i<bitboards.size(); i++){
    board |= bitboards[i];
  }
  return board;
}

void printBitboard(std::vector<__uint128_t> pieces){
  char printArr[17][17];
  for (int i=0; i<17; i++){
    for (int j=0; j<17; j++){
      printArr[i][j] = ' ';
    }
  }
  for (int i=0; i<playersInGame; i++){
    uint64_t low = (uint64_t)pieces[i];
    uint64_t high = (uint64_t)(pieces[i] >> 64);
    while (low){
      int trailing_zeros = __builtin_ctzll(low);//supported by gcc
      low &= low-1; //clear that set bit
      std::pair<uint8_t,uint8_t> ind = bitToIndices[trailing_zeros];
      printArr[ind.second][ind.first] = i;
    }
    while (high){
      int trailing_zeros = __builtin_ctzll(high)+64;//add 64 for high address
      high &= high-1;
      std::pair<uint8_t,uint8_t> ind = bitToIndices[trailing_zeros];
      printArr[ind.second][ind.first] = i;
    }
  }
  printBoard(printArr);
}

bool validMove(__uint128_t occupied, __uint128_t playerPieces, std::pair<uint8_t,uint8_t> move){
  auto bitIsSet = [](__uint128_t n, uint8_t bit) -> bool {
    return (n & ((__uint128_t)1 << bit) ) ;
  };
  if ( move.first > 120 || move.second > 120)
  {
    return false;
  }
  if (
    bitIsSet(playerPieces,move.first) && !bitIsSet(playerPieces,move.second) &&
    bitIsSet(occupied, move.first) && !bitIsSet(occupied, move.second))
    {
      return true;
    }

  return false;
}

int generatePath(uint8_t jumpArr[64],std::pair<uint8_t,uint8_t> move, __uint128_t occupied, int jumpsMade,
__uint128_t jumpedTo){
  jumpArr[jumpsMade] = move.first;

  auto setBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n | ((__uint128_t)1 << bit));
  };
  auto unsetBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n & (~((__uint128_t)1 << bit)));
  };
  auto bitIsSet = [](__uint128_t n, int bit) -> bool {
    return (n & ((__uint128_t)1 << bit) ) ;
  };

  // base case: we have moved the piece all the way to its destination
  if (move.first ==  move.second){
    return 1;
  }
  
  if (jumpsMade == 0){
    //Generate one-space moves
    __uint128_t pieceMoves = moves[move.first] & (~occupied);
    //find set bits to get possible piece squares
    //split 128 bit uint into 2 64 bit uints
    uint64_t low = (uint64_t)pieceMoves;
    uint64_t high = (uint64_t)(pieceMoves >> 64);
    while (low){
      int trailing_zeros = __builtin_ctzll(low);//supported by gcc
      low &= low-1; //clear that set bit
      //check if we can do an immediate hop to the end space
      if (trailing_zeros == move.second){
        jumpArr[1] = move.second;
        return 2;
      }
    }
    while (high){
      int trailing_zeros = __builtin_ctzll(high) + 64;//add 64 for high register
      high &= high-1;
      if (trailing_zeros == move.second){
        jumpArr[1] = move.second;
        return 2;
      }
    }
  }

  //Generate jumps
  // "hashmap" to check if we already jumped to a spot
  jumpedTo = setBit(jumpedTo, move.first);

  //add all possible jumps to stack
  //first compute jumps and then check if their intermediate space is occupied to verify
  __uint128_t pieceJumps = jumps[move.first] & (~occupied);
  uint64_t low = (uint64_t)pieceJumps;
  uint64_t high = (uint64_t)(pieceJumps >> 64);
  while (low){
    int trailing_zeros = __builtin_ctzll(low);
    low &= low-1;
    if (bitIsSet(occupied,halfJumps[move.first][trailing_zeros])  && bitIsSet(~jumpedTo,trailing_zeros)){
      jumpArr[jumpsMade+1] = trailing_zeros;
      int jumpsToEnd = generatePath(jumpArr, {trailing_zeros,move.second}, occupied, jumpsMade+1, jumpedTo);
      if (jumpsToEnd != -1){
        return jumpsToEnd + 1;
      }
    }        
  }
  while (high){
    int trailing_zeros = __builtin_ctzll(high) + 64;//add 64 for high address
    high &= high-1;
    //check that there is a piece to jump over && we have not visited the landing space on our search
    if (bitIsSet(occupied,halfJumps[move.first][trailing_zeros])  && bitIsSet(~jumpedTo,trailing_zeros)){
      jumpArr[jumpsMade+1] = trailing_zeros;
      int jumpsToEnd = generatePath(jumpArr, {trailing_zeros,move.second}, occupied, jumpsMade+1, jumpedTo);
      if (jumpsToEnd != -1){
        return jumpsToEnd + 1;
      }
    }
  }

  return -1;
}