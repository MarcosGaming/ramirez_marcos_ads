#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game_board.h"


// Forward declaration of functions
void Initialise(struct GameBoard*);
void Draw(struct GameBoard*);
void GameOver(struct GameBoard*);
void Update(struct GameBoard*);
void positionSelection(struct GameBoard*, int*);

// TIC TAC TOE / CONNECT 4 
int main( int argc, const char* argv[])
{
	// Game board that is going to be used during all the game
	struct GameBoard gameBoard;

	// Load the game elements
	Initialise(&gameBoard);
	Draw(&gameBoard);

	// Game Loop
	while (1)
	{
		Update(&gameBoard);
		Draw(&gameBoard);
		GameOver(&gameBoard);
	}

	return 0;
}


// Initialises the game
void Initialise(struct GameBoard* gameBoard)
{
	printf("\n");
	printf("Welcome, please enter your preference settings to start playing.\n");
	printf("-------------------------------------------------------------------------------------\n");
	// Select the game board size, this can be eithter 3x3 or 7x6
	int columns;
	int rows;
	int winCount;
	while (1)
	{
		printf("Select game type: Enter 1 for Tic Tac Toe | Enter 2 for Connect Four \n");
		int selection;
		scanf("%d", &selection);
		if (selection == 1)
		{
			columns = 3;
			rows = 3;
			winCount = 3;
			break;
		}
		else if (selection == 2)
		{
			columns = 7;
			rows = 6;
			winCount = 4;
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
	setUpGame(gameBoard, winCount, columns, rows, namePlayer1, namePlayer2, piecePlayer1, piecePlayer2);
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
					// Only the cell closest to the left has a | at the beginning
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
					// Only the cell closest to the left has a | at the beginning
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
				// Only the cell closest to the left has a | at the beginning
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

// Checks if the game is over
void GameOver(struct GameBoard* gameBoard)
{
	if (gameBoard->gameOver)
	{
		// Someone has won the game
		if (gameBoard->winnerName != '\0')
		{
			printf("\n	GAME OVER: %s IS THE WINNER\n\n", gameBoard->winnerName);
		}
		else
		{
			printf("\n	GAME OVER: DRAW\n\n");
		}
		while (1)
		{
			printf("Enter 1 to play again | Enter 2 to exit the game\n");
			int selection;
			scanf("%d", &selection);
			if (selection == 1)
			{
				playAgain(gameBoard);
				// Draw the initial game board again
				Draw(gameBoard);
				break;
			}
			else if (selection == 2)
			{
				exit(0);
			}
			printf("Selection incorrect, please enter either 1 or 2\n");
			// Clear stdin buffer to avoid infinite loop if string is entered
			fseek(stdin, 0, SEEK_END);
		}
	}
}

void Update(struct GameBoard* gameBoard)
{
	for (int i = 0; i < 2; i++)
	{
		// Ask player to select position to place its piece
		printf("%s select position to place %c : ", gameBoard->players[i].name, gameBoard->players[i].piece);
		// Make sure that position enter by player is valid
		int position = 0;
		positionSelection(gameBoard, &position);
		// Update grid
		updateGrid(gameBoard, gameBoard->players[i].piece, position);
		// Check for winner
		winnerCheck(gameBoard);
		// The winner is the player that performed the last move
		if (gameBoard->gameOver)
		{
			gameBoard->winnerName = gameBoard->players[i].name;
			break;
		}
		// Check for no longer positions available
		availablePosCheck(gameBoard);
		if (gameBoard->gameOver)
		{
			break;
		}
	}
}

// Function to assert that the position entered is correct
void positionSelection(struct GameBoard* gameBoard, int* position)
{
	while(1)
	{
		scanf("%d", position);
		// Map the position entered to grid location
		int x = (*position - 1) / gameBoard->columns;
		int y = (*position - 1) % gameBoard->columns;
		// Position out of range or already taken
		if (*position <= 0 || *position > (gameBoard->columns * gameBoard->rows) || gameBoard->grid[x][y] != '\0')
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
}