#ifndef GAME_BOARD
#define GAME_BOARD

#include <string.h>
// Player struct
struct Player
{
	char* name; // Name of the player
	char piece; // Noughts or crosses, 'O' or 'X'
};

// GameBoard struct
struct GameBoard
{
	char** grid;			// Positions
	int columns;				// Number of columns
	int rows;				// Number of rows 
	struct Player player1;	// Player 1
	struct Player player2;	// Player 2
};

// GameBoard API

// Initialise gameboard
void setUpGame(struct GameBoard* gameBoard, int columns, int rows, char* name1, char* name2, char piece1, char piece2);
void positionSelect(int* position);

#endif