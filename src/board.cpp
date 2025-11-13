#include "board.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>


//must call setMovesAndJumps() to initiallize this
//key: position(0-121) value: bitboard (set to 1 at valid move location)
std::unordered_map<int, __uint128_t> moves;

//must call setMovesAndJumps() to initiallize this
//key: position(0-121) value: bitboard (set to 1 at valid jump location)
std::unordered_map<int, __uint128_t> jumps;


// '<int>' is the player's pieces
// ' ' is an empty space
char startBoard[BOARD_DIM][BOARD_DIM] = {
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', 0 ,' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', 0 , 0 ,' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', 0 , 0 , 0 ,' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ', 0 , 0 , 0 , 0 ,' ',' ',' ',' '},
  {' ',' ',' ',' ', 1 , 1 , 1 , 1 ,' ',' ',' ',' ',' ', 5 , 5 , 5 , 5 },
  {' ',' ',' ',' ', 1 , 1 , 1 ,' ',' ',' ',' ',' ',' ', 5 , 5 , 5 ,' '},
  {' ',' ',' ',' ', 1 , 1 ,' ',' ',' ',' ',' ',' ',' ', 5 , 5 ,' ',' '},
  {' ',' ',' ',' ', 1 ,' ',' ',' ',' ',' ',' ',' ',' ', 5 ,' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ', 2 ,' ',' ',' ',' ',' ',' ',' ',' ', 4 ,' ',' ',' ',' '},
  {' ',' ', 2 , 2 ,' ',' ',' ',' ',' ',' ',' ', 4 , 4 ,' ',' ',' ',' '},
  {' ', 2 , 2 , 2 ,' ',' ',' ',' ',' ',' ', 4 , 4 , 4 ,' ',' ',' ',' '},
  { 2 , 2 , 2 , 2 ,' ',' ',' ',' ',' ', 4 , 4 , 4 , 4 ,' ',' ',' ',' '},
  {' ',' ',' ',' ', 3 , 3 , 3 , 3 ,' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ', 3 , 3 , 3 ,' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ', 3 , 3 ,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ', 3 ,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
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
std::unordered_map <int, std::pair<int,int>> bitToIndices = {
  {0,std::pair<int,int>(12,0)},{1,std::pair<int,int>(11,1)},{2,std::pair<int,int>(12,1)},        
  {3,std::pair<int,int>(10,2)},{4,std::pair<int,int>(11,2)},{5,std::pair<int,int>(12,2)},        
  {6,std::pair<int,int>(9,3)},{7,std::pair<int,int>(10,3)},{8,std::pair<int,int>(11,3)},
  {9,std::pair<int,int>(12,3)},{10,std::pair<int,int>(4,4)},{11,std::pair<int,int>(5,4)},        
  {12,std::pair<int,int>(6,4)},{13,std::pair<int,int>(7,4)},{14,std::pair<int,int>(8,4)},        
  {15,std::pair<int,int>(9,4)},{16,std::pair<int,int>(10,4)},{17,std::pair<int,int>(11,4)},      
  {18,std::pair<int,int>(12,4)},{19,std::pair<int,int>(13,4)},{20,std::pair<int,int>(14,4)},     
  {21,std::pair<int,int>(15,4)},{22,std::pair<int,int>(16,4)},{23,std::pair<int,int>(4,5)},      
  {24,std::pair<int,int>(5,5)},{25,std::pair<int,int>(6,5)},{26,std::pair<int,int>(7,5)},        
  {27,std::pair<int,int>(8,5)},{28,std::pair<int,int>(9,5)},{29,std::pair<int,int>(10,5)},       
  {30,std::pair<int,int>(11,5)},{31,std::pair<int,int>(12,5)},{32,std::pair<int,int>(13,5)},     
  {33,std::pair<int,int>(14,5)},{34,std::pair<int,int>(15,5)},{35,std::pair<int,int>(4,6)},      
  {36,std::pair<int,int>(5,6)},{37,std::pair<int,int>(6,6)},{38,std::pair<int,int>(7,6)},        
  {39,std::pair<int,int>(8,6)},{40,std::pair<int,int>(9,6)},{41,std::pair<int,int>(10,6)},       
  {42,std::pair<int,int>(11,6)},{43,std::pair<int,int>(12,6)},{44,std::pair<int,int>(13,6)},     
  {45,std::pair<int,int>(14,6)},{46,std::pair<int,int>(4,7)},{47,std::pair<int,int>(5,7)},       
  {48,std::pair<int,int>(6,7)},{49,std::pair<int,int>(7,7)},{50,std::pair<int,int>(8,7)},        
  {51,std::pair<int,int>(9,7)},{52,std::pair<int,int>(10,7)},{53,std::pair<int,int>(11,7)},      
  {54,std::pair<int,int>(12,7)},{55,std::pair<int,int>(13,7)},{56,std::pair<int,int>(4,8)},      
  {57,std::pair<int,int>(5,8)},{58,std::pair<int,int>(6,8)},{59,std::pair<int,int>(7,8)},        
  {60,std::pair<int,int>(8,8)},{61,std::pair<int,int>(9,8)},{62,std::pair<int,int>(10,8)},       
  {63,std::pair<int,int>(11,8)},{64,std::pair<int,int>(12,8)},{65,std::pair<int,int>(3,9)},      
  {66,std::pair<int,int>(4,9)},{67,std::pair<int,int>(5,9)},{68,std::pair<int,int>(6,9)},        
  {69,std::pair<int,int>(7,9)},{70,std::pair<int,int>(8,9)},{71,std::pair<int,int>(9,9)},        
  {72,std::pair<int,int>(10,9)},{73,std::pair<int,int>(11,9)},{74,std::pair<int,int>(12,9)},     
  {75,std::pair<int,int>(2,10)},{76,std::pair<int,int>(3,10)},{77,std::pair<int,int>(4,10)},     
  {78,std::pair<int,int>(5,10)},{79,std::pair<int,int>(6,10)},{80,std::pair<int,int>(7,10)},     
  {81,std::pair<int,int>(8,10)},{82,std::pair<int,int>(9,10)},{83,std::pair<int,int>(10,10)},    
  {84,std::pair<int,int>(11,10)},{85,std::pair<int,int>(12,10)},{86,std::pair<int,int>(1,11)},   
  {87,std::pair<int,int>(2,11)},{88,std::pair<int,int>(3,11)},{89,std::pair<int,int>(4,11)},     
  {90,std::pair<int,int>(5,11)},{91,std::pair<int,int>(6,11)},{92,std::pair<int,int>(7,11)},     
  {93,std::pair<int,int>(8,11)},{94,std::pair<int,int>(9,11)},{95,std::pair<int,int>(10,11)},    
  {96,std::pair<int,int>(11,11)},{97,std::pair<int,int>(12,11)},{98,std::pair<int,int>(0,12)},   
  {99,std::pair<int,int>(1,12)},{100,std::pair<int,int>(2,12)},{101,std::pair<int,int>(3,12)},   
  {102,std::pair<int,int>(4,12)},{103,std::pair<int,int>(5,12)},{104,std::pair<int,int>(6,12)},  
  {105,std::pair<int,int>(7,12)},{106,std::pair<int,int>(8,12)},{107,std::pair<int,int>(9,12)},  
  {108,std::pair<int,int>(10,12)},{109,std::pair<int,int>(11,12)},{110,std::pair<int,int>(12,12)},
  {111,std::pair<int,int>(4,13)},{112,std::pair<int,int>(5,13)},{113,std::pair<int,int>(6,13)},  
  {114,std::pair<int,int>(7,13)},{115,std::pair<int,int>(4,14)},{116,std::pair<int,int>(5,14)},  
  {117,std::pair<int,int>(6,14)},{118,std::pair<int,int>(4,15)},{119,std::pair<int,int>(5,15)},  
  {120,std::pair<int,int>(4,16)}  
};

//refer to diagram for mapping explanation
void setMovesAndJumps(){
  auto isInBounds = [](int x, int y) -> bool {
    return x>=0 && y>=0 && x<BOARD_DIM && y<BOARD_DIM && inBounds[x][y];
  };
  auto setBit = [](__uint128_t n, int bit) -> __uint128_t {
    return (n | ((__uint128_t)1 << bit));
  };

  int moveDirs[6][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,-1}};
  int jumpDirs[6][2] = { {2,0},{-2,0},{0,2},{0,-2},{2,2},{-2,-2}};

  //create backwards map for this function
  std::map<std::pair<int,int>, int> indicesToBit;
  for (int i=0; i<SPACE_AMOUNT; i++){
    std::pair<int, int> p = bitToIndices[i];
    indicesToBit[p] = i;
  }
  
  //for every space, map out the moves and jumps you can make from there
  for (int i=0;i<SPACE_AMOUNT;i++){
    std::pair<int, int> p = bitToIndices[i];
    int x = p.first;
    int y = p.second;
    std::vector<int> moves_possible, jumps_possible;
    for (int j=0; j<6; j++){
      int newX = x+moveDirs[j][0];
      int newY = y+moveDirs[j][1];
      if (isInBounds(newX, newY)){
        moves_possible.push_back(indicesToBit[std::pair<int,int>(newX,newY)]);
      }

      newX = x+jumpDirs[j][0];
      newY = y+jumpDirs[j][1];
      if (isInBounds(newX, newY)){
        jumps_possible.push_back(indicesToBit[std::pair<int,int>(newX,newY)]);
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

}
