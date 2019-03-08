#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_board.h"


// Forward declaration of functions
void Initialise(struct GameBoard* game);
void Draw(struct GameBoard* gameBoard);
void Update(struct GameBoard* gameBoard);

// TIC TAC TOE GAME
int main( int argc, const char* argv[])
{
	// Game board that is going to be used during all the game
	struct GameBoard tic_tac_toe;

	// Load the game elements
	Initialise(&tic_tac_toe);

	// Game Loop
	while (1)
	{
		Draw(&tic_tac_toe);
		Update(&tic_tac_toe);
	}

	return 0;
}


// Initialises the game
void Initialise(struct GameBoard* gameBoard)
{
	printf("\n");
	printf("Welcome to TIC_TAC_TOE, please enter your preference settings to start playing.\n");
	printf("-------------------------------------------------------------------------------------\n");
	// Select the game board size, this can be eithter 3x3 or 7x6
	int columns;
	int rows;
	while (1)
	{
		printf("Select game type: Enter 1 for Tic Tac Toe | Enter 2 for Connect Four \n");
		int selection;
		scanf("%d", &selection);
		if (selection == 1)
		{
			columns = 3;
			rows = 3;
			break;
		}
		else if (selection == 2)
		{
			columns = 7;
			rows = 6;
			break;
		}
		printf("Selection incorrect, please introduce either 1 or 2\n\n");
		// Clear stdin buffer to avoid infinite loop if string is entered
		fseek(stdin, 0, SEEK_END);
	}
	printf("-------------------------------------------------------------------------------------\n");

	// Player 1 elements
	char namePlayer1[256];
	char piecePlayer1;
	// Player 2 elements
	char namePlayer2[256];
	char piecePlayer2;

	// Enter Players name
	printf("Enter player 1 name: ");
	scanf("%s", namePlayer1);
	printf("Enter player 2 name: ");
	scanf("%s", namePlayer2);
	printf("-------------------------------------------------------------------------------------\n");

	// Select pieces
	while (1)
	{
		printf("Select player 1 piece: Enter X for crosses | Enter O for noughts\n");
		char selection;
		scanf(" %c", &selection);
		if (selection == 'X')
		{
			piecePlayer1 = 'X';
			piecePlayer2 = 'O';
			break;
		}
		else if (selection == 'O')
		{
			piecePlayer1 = 'O';
			piecePlayer2 = 'X';
			break;
		}
		printf("Selection incorrect, please introduce either X or O\n\n");
		// Clear stdin buffer to avoid infinite loop if string is entered
		fseek(stdin, 0, SEEK_END);
	}
	printf("-------------------------------------------------------------------------------------\n");

	// Print players and corresponding pieces
	printf("Player1: %s with %c\n", namePlayer1, piecePlayer1);
	printf("Player2: %s with %c\n", namePlayer2, piecePlayer2);
	// Print game mode
	if (columns == 3)
	{
		printf("TIC_TAC_TOE\n\nTo win you will need to place your piece three times in the same row either vertically, horizontally or diagonally.\n\n");
	}
	else
	{
		printf("CONNECT 4\n\nTo win you will need to place your piece four times in the same row either vertically, horizontally or diagonally.\n\n");
	}
	printf("GOOD LUCK AND HAVE FUN\n");
	printf("-------------------------------------------------------------------------------------\n");

	// Set up game with the parameters selected
	setUpGame(gameBoard, columns, rows, namePlayer1, namePlayer2, piecePlayer1, piecePlayer2);
}

// Draw the gameboard grid based on its size with the pieces in the positions selected by the players
void Draw(struct GameBoard* gameBoard)
{
	int counter = 1;
	printf("\n");
	for (int i = 0; i < gameBoard->rows; i++)
	{
		// First line of cell
		for (int j = 0; j < gameBoard->columns; j++)
		{
			// Onlt the cell closest to the left has a | at the beginning
			if (j == 0)
			{
				printf("	|      |");
			}
			else
			{
				printf("      |");
			}
		}
		printf("\n");
		// Second line of cell
		for (int j = 0; j < gameBoard->columns; j++)
		{
			// Print number
			if (gameBoard->grid[i][j] == '\0')
			{
				// 2 digit number need more space
				if (counter <= 9)
				{
					// Onlt the cell closest to the left has a | at the beginning
					if (j == 0)
					{
						printf("	|  %d   |", counter);
					}
					else
					{
						printf("  %d   |", counter);
					}
				}
				else
				{
					// Onlt the cell closest to the left has a | at the beginning
					if (j == 0)
					{
						printf("	|  %d  |", counter);
					}
					else
					{
						printf("  %d  |", counter);
					}
				}
			}
			// Print piece
			else
			{
				// Onlt the cell closest to the left has a | at the beginning
				if(j == 0)
				{
					printf("	|  %c   |", gameBoard->grid[i][j]);
				}
				else
				{
					printf("  %c   |", gameBoard->grid[i][j]);
				}
			}
			counter++;
		}
		printf("\n");
		// Third line of cell
		for (int j = 0; j < gameBoard->columns; j++)
		{
			// Not last cells
			if (i != gameBoard->rows - 1)
			{
				// Onlt the cell closest to the left has a | at the beginning
				if (j == 0)
				{
					printf("	|______|");
				}
				else
				{
					printf("______|");
				}
			}
			// Last cells
			else
			{
				// Onlt the cell closest to the left has a | at the beginning
				if (j == 0)
				{
					printf("	|      |");
				}
				else
				{
					printf("      |");
				}
			}
		}
		printf("\n");
	}
	printf("\n");
}

void Update(struct GameBoard* gameBoard)
{
	// Ask player1 to select position to place its piece
	printf("%s select position to place %c : ", gameBoard->player1.name, gameBoard->player1.piece);
	// Make sure that position enter by player1 is valid
	int position1 = 0;
	while (1)
	{
		scanf("%d", position1);
		// Map the position entered to grid location
		int x = (position1 - 1) / gameBoard->rows;
		int y = (position1 - 1) % gameBoard->rows;
		// Position out of range or already taken
		if (position1 <= 0 || position1 > (gameBoard->columns * gameBoard->rows) || gameBoard->grid[x][y] != '\0')
		{
			printf("Position entered not valid, please choose a valid position.\n");
		}
		else
		{
			break;
		}
		// Clear stdin buffer to avoid infinite loop if string is entered
		fseek(stdin, 0, SEEK_END);
	}
	// Update grid

	// Check for winner

	// Ask player2 to select position to place its piece

	// Check that position enter by player2 is correct

	// Update grid

	// Check for winner
}