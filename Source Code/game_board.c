#include "game_board.h"
#include <stdlib.h>
#include <stdio.h>

// Initialise gameboard
void setUpGame(struct GameBoard* gameBoard, int winCount, int columns, int rows, char* name1, char* name2, char piece1, char piece2)
{
	// Set game board parameters
	gameBoard->columns = columns;
	gameBoard->rows = rows;
	gameBoard->winCount = winCount;
	gameBoard->gameOver = false;
	gameBoard->winnerName = '\0';

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
	// The first player (player 0) is always going to be the crosses one
	int player1;
	int player2;
	if (piece1 == 'X')
	{
		player1 = 0;
		player2 = 1;
	}
	else
	{
		player1 = 1;
		player2 = 0;
	}
	// Allocate memory for the player names
	gameBoard->players[player1].name = (char*)malloc(name1Size * sizeof(char));
	gameBoard->players[player2].name = (char*)malloc(name2Size * sizeof(char));

	// Set the player names of the gameboard
	strncpy(gameBoard->players[player1].name, name1, name1Size);
	strncpy(gameBoard->players[player2].name, name2, name2Size);

	// Set the player pieces
	gameBoard->players[player1].piece = piece1;
	gameBoard->players[player2].piece = piece2;
}

// Update the grid placing the player piece in the selected position
void updateGrid(struct GameBoard* gameBoard, char piece, int position)
{
	// Get the corresponding position in the grid
	int x = (position - 1) / gameBoard->columns;
	int y = (position - 1) % gameBoard->columns;
	// Set the piece value to the grid position
	gameBoard->grid[x][y] = piece;
}

// Check for winner
void winnerCheck(struct GameBoard* gameBoard)
{
	// Vertical check
	for (int column = 0; column < gameBoard->columns; column++)
	{
		int counter = 1;
		char temPiece = '\0';
		for (int row = 0; row < gameBoard->rows; row++)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if(temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
		}
	}

	// Horizontal check
	for (int row = 0; row < gameBoard->rows; row++)
	{
		int counter = 1;
		char temPiece = '\0';
		for (int column = 0; column < gameBoard->columns; column++)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if (temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
		}
	}

	// Diagonal check
	if (gameBoard->winCount == 3)
	{
		ticTacToeDiagonalCheck(gameBoard);
	}
	else
	{
		connect4DiagonalCheck(gameBoard);
	}
}

// In tic tac toe only the middle diagonal needs to be checked
void ticTacToeDiagonalCheck(struct GameBoard* gameBoard)
{
	// Right diagonal
	int counter = 1;
	char temPiece = '\0';
	for (int i = 0; i < gameBoard->rows; i++)
	{
		// If there is a '\0' in that cell stop the check
		if (gameBoard->grid[i][i] == '\0')
		{
			break;
		}
		// If the temPiece is empty initialise it
		if (temPiece == '\0')
		{
			temPiece = gameBoard->grid[i][i];
		}
		// If the temPiece is not equal to the one in the cell being checked then stop the check
		else if (temPiece != gameBoard->grid[i][i])
		{
			break;
		}
		else
		{
			counter++;
		}
		// Win
		if (counter == gameBoard->winCount)
		{
			gameBoard->gameOver = true;
		}
	}

	// Left diagonal
	counter = 1;
	temPiece = '\0';
	int column = 0;
	for (int row = gameBoard->rows - 1; row >= 0; row--)
	{
		// If there is a '\0' in that cell stop the check
		if (gameBoard->grid[row][column] == '\0')
		{
			break;
		}
		// If the temPiece is empty initialise it
		if (temPiece == '\0')
		{
			temPiece = gameBoard->grid[row][column];
			column++;
		}
		// If the temPiece is not equal to the one in the cell being checked then stop the check
		else if (temPiece != gameBoard->grid[row][column])
		{
			break;
		}
		else
		{
			column++;
			counter++;
		}
		// Win
		if (counter == gameBoard->winCount)
		{
			gameBoard->gameOver = true;
		}
	}
}

// In connect 4, 12 diagonals need to be check in total
void connect4DiagonalCheck(struct GameBoard* gameBoard)
{
	// Right diagonals left half
	for (int initialRow = 2; initialRow >= 0; initialRow--)
	{
		int column = 0;
		int counter = 1;
		char temPiece = '\0';
		for (int row = initialRow; row < gameBoard->rows; row++)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if (temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
			column++;
		}
	}
	// Right diagonals right half
	for (int initialColumn = 1; initialColumn <= 3; initialColumn++)
	{
		int row = 0;
		int counter = 1;
		char temPiece = '\0';
		for (int column = initialColumn; column < gameBoard->columns; column++)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if (temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
			row++;
		}
	}

	// Left diagonals left half
	for (int initialColumn = 3; initialColumn <= 5; initialColumn++)
	{
		int row = 0;
		int counter = 1;
		char temPiece = '\0';
		for (int column = initialColumn; column >= 0; column--)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if (temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
			row++;
		}
	}
	// Left diagonals right half
	for (int initialRow = 2; initialRow >= 0; initialRow--)
	{
		int column = gameBoard->columns-1;
		int counter = 1;
		char temPiece = '\0';
		for (int row = initialRow; row < gameBoard->rows; row++)
		{
			// If there is a different piece in the current position change temp piece and reset counter
			if (temPiece != gameBoard->grid[row][column])
			{
				temPiece = gameBoard->grid[row][column];
				counter = 1;
			}
			else if (temPiece == gameBoard->grid[row][column] && temPiece != '\0')
			{
				counter++;
			}
			// Win
			if (counter == gameBoard->winCount)
			{
				gameBoard->gameOver = true;
			}
			column--;
		}
	}
}

// Checks for positions available
void availablePosCheck(struct GameBoard* gameBoard)
{
	for (int i = 0; i < gameBoard->rows; i++)
	{
		for (int j = 0; j < gameBoard->columns; j++)
		{
			// Return if we find an available position
			if (gameBoard->grid[i][j] == '\0')
			{
				return;
			}
		}
	}
	// Set game over to true if there are no available positions
	gameBoard->gameOver = true;
}

// Restarts the game with the same game type and players
void playAgain(struct GameBoard* gameBoard)
{
	// Resets some game parameters
	gameBoard->gameOver = false;
	gameBoard->winnerName = '\0';
	// Reset grid positions
	for (int i = 0; i < gameBoard->rows; i++)
	{
		for (int j = 0; j < gameBoard->columns; j++)
		{
			gameBoard->grid[i][j] = '\0';
		}
	}
}