#pragma once

#define DEFAULT_DEPTH 2

//plays the game. starts at default position for 6 players
//inputs: 
// quit - stop program
// restart <playerAmount> - restart the game, default 6 players
// go <depth> - find the best move at a depth. default is DEFAULT_DEPTH
// move <space 1> <space 2> - advance the position by moving the piece from space 1 to 2. must be valid
// board - print the ASCII board
// index - print the ASCII board with indices labeled
// help - command information
void play();