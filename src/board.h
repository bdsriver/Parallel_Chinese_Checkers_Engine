#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#define MAX_PLAYER_AMOUNT 6
#define BOARD_DIM 17
#define PLAYER_PIECE_AMOUNT 10
#define SPACE_AMOUNT 121
#define MAX_MOVES 320

//Amount of players in the current game
extern int playersInGame;
//access space (x,y) with inBounds[y][x]
extern bool inBounds[][BOARD_DIM];
extern std::unordered_map<int, std::pair<uint8_t,uint8_t>> bitToIndices;
extern __uint128_t moves[SPACE_AMOUNT];
extern __uint128_t jumps[SPACE_AMOUNT];
//mapping of the location of the end of the jump to the location of the intermediate space
extern std::vector<std::unordered_map<int,int>> halfJumps;
//piece table value per space
extern std::vector<std::vector<float>> pieceValues;
extern std::vector<std::vector<int>> startPoints;

//the 10 goal spots for each start player
//0: start:0-9, end:111-120 
// see diagram
// "front" of the end zone spaces are the first 4 ints of each vector
extern std::vector<std::vector<int>> endZones;

//refer to diagram for mapping explanation
//must be called to set the global moves,pieces, and jumps tables
void setMovesAndJumps(int playerAmount=6);  

//return the amount of moves generated into the moveArr array
//occupied is a bitboard of piece locations on the board
//pieces is a bitboard of only the turn player's piece
//return -1 if it is a winning position
int generateMoves(std::pair<uint8_t,uint8_t> moveArr[MAX_MOVES] ,__uint128_t occupied, __uint128_t pieces, int player);

// change the "occipied" bitboard and piece pos to represent the new position
void makeMove(__uint128_t *occupied, std::pair<uint8_t,uint8_t> move);

//change the "occupied" bitboard and piece pos back after calling makeMove() and exploring tree
void unMakeMove(__uint128_t *occupied, std::pair<uint8_t,uint8_t> move);

//return true if the player with playerPieces can make the move or not
bool validMove(__uint128_t occupied, __uint128_t playerPieces, std::pair<uint8_t,uint8_t> move);

void printBoard(char board[][17]);

//generate a bitboard for pices of each player's pieces
std::vector<__uint128_t> pieceVectorToBitboards(std::vector<std::vector<int>> pieces);

//generate a bitboard for if a space on the board is occupied
__uint128_t pieceVectorToBoard(std::vector<std::vector<int>> pieces);

void printBitboard(std::vector<__uint128_t> pieces);

//Return an int with the amount of jumps until the piece reaches the spot
// Returns -1 if it is not possible
// jumpArr will be changed to an array with the entire path it takes to complete a move
// recursive function. jumpsMade and jumpedTo do not need to be given arguments
int generatePath(uint8_t jumpArr[64],std::pair<uint8_t,uint8_t> move, __uint128_t occupied, int jumpsMade=0,
__uint128_t jumpedTo=0);