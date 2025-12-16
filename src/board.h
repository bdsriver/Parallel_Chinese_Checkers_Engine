#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#define PLAYER_AMOUNT 6
#define BOARD_DIM 17
#define PLAYER_PIECE_AMOUNT 10
#define SPACE_AMOUNT 121

//access space (x,y) with startBoard[y][x]
extern char startBoard[][BOARD_DIM];
//access space (x,y) with inBounds[y][x]
extern bool inBounds[][BOARD_DIM];
extern std::unordered_map<int, std::pair<int,int>> bitToIndices;
extern std::vector<__uint128_t> moves;
extern std::vector<__uint128_t> jumps;
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
void setMovesAndJumps();

//copy the original onto the copy
void copyBoard(char original[][BOARD_DIM], char copy[][BOARD_DIM]);

//return a vector of {space1, space2} pairs
//occupied is a bitboard of piece locations on the board
//pieces is a map of {<0-9>,<piece location>} for the player's piece
std::vector<std::pair<int,int>> generateMoves( __uint128_t occupied, __uint128_t pieces);

// return a vector with the locations of each piece
std::vector<int> initPieces(char board[][BOARD_DIM], char playerID);

// return the bitboard that represents if a piece is at each space
__uint128_t boardToOccupiedBitboard(char board[][BOARD_DIM]);

// change the "occipied" bitboard and piece pos to represent the new position
void makeMove(__uint128_t *occupied, std::pair<int,int> move, int *piecePos);

//change the "occupied" bitboard and piece pos back after calling makeMove() and exploring tree
void unMakeMove(__uint128_t *occupied, std::pair<int,int> move, int *piecePos);

// change the "occipied" bitboard and piece pos to represent the new position
void makeMove(__uint128_t *occupied, std::pair<int,int> move);

//change the "occupied" bitboard and piece pos back after calling makeMove() and exploring tree
void unMakeMove(__uint128_t *occupied, std::pair<int,int> move);

void printBoard(char board[][17]);

//generate a bitboard for pices of each player's pieces
std::vector<__uint128_t> pieceVectorToBitboards(std::vector<std::vector<int>> pieces);