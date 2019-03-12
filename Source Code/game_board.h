#ifndef GAME_BOARD
#define GAME_BOARD

#include <string.h>
#include <stdbool.h>
// Player struct
struct Player
{
	char* name; // Name of the player
	char piece; // Noughts or crosses, 'O' or 'X'
};

// GameBoard struct
struct GameBoard
{
	char** grid;				// Positions
	bool gameOver;				// Boolean to terminate the game
	int winCount;				// 3 for tic tac toe, 4 for connect 4
	int columns;				// Number of columns
	int rows;					// Number of rows 
	struct Player players[2];	// Players
	char* winnerName;			// Name of the winner player
};

// GameBoard API

// Initialise gameboard
void setUpGame(struct GameBoard* gameBoard, int winCount, int columns, int rows, char* name1, char* name2, char piece1, char piece2);
// Update the grid placing the player piece in the selected position
void updateGrid(struct GameBoard* gameBoard, char piece, int position);
// Check for winner
void winnerCheck(struct GameBoard* gameBoard);
// Diagonal checks are different based on the game type for optimitation
void ticTacToeDiagonalCheck(struct GameBoard* gameBoard);
void connect4DiagonalCheck(struct GameBoard* gameBoard);
// Checks for positions available
void availablePosCheck(struct GameBoard* gameBoard);
// Restarts the game but with same game type and players
void playAgain(struct GameBoard* gameBoard);

#endif