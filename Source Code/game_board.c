#include "game_board.h"
#include <stdlib.h>
#include <stdio.h>

// Initialise gameboard
void setUpGame(struct GameBoard* gameBoard, int columns, int rows, char* name1, char* name2, char piece1, char piece2)
{
	// Allocate memory for the grid
	gameBoard->grid = (char**)malloc(rows * sizeof(char*));
	for (int i = 0; i < rows; i++)
	{
		gameBoard->grid[i] = (char*)malloc(columns * sizeof(char));

	}

	// All positions are empty at the beginning
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			gameBoard->grid[i][j] = '\0';
		}
	}

	// Set grid size property
	gameBoard->columns = columns;
	gameBoard->rows = rows;

	// Resize the array names to not waste memory
	int name1Size = 0;
	int name2Size = 0;
	// Get size of names
	for (int i = 0; name1[i] != '\0'; i++)
	{
		name1Size++;
	}
	for (int i = 0; name2[i] != '\0'; i++)
	{
		name2Size++;
	}
	// Need to add one more int to the size for the null terminator
	name1Size++;
	name2Size++;
	// Allocate memory for the player names
	gameBoard->player1.name = (char*)malloc(name1Size * sizeof(char));
	gameBoard->player2.name = (char*)malloc(name2Size * sizeof(char));
	
	// Set the player names of the gameboard
	strncpy(gameBoard->player1.name, name1, name1Size);
	strncpy(gameBoard->player2.name, name2, name2Size);
	
	// Set the player pieces
	gameBoard->player1.piece = piece1;
	gameBoard->player2.piece = piece2;
}

void positionSelect(int* position)
{

}