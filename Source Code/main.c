#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "game_board.h"
#include "storage.h"


// Forward declaration of functions
void MainMenu();
void RecordingsMenu();
void Initialise(struct GameBoard*);
void Draw(struct GameBoard*);
void GameOver(struct GameBoard*, struct Deque*, struct Stack*, struct Stack*, bool*);
void Update(struct GameBoard*, struct Deque*, struct Stack*, struct Stack*, bool);
void NormalUpdate(struct GameBoard*, struct Deque*, struct Stack*, struct Stack*, int);
void AIUpdate(struct GameBoard*, struct Deque*, int);
void positionSelection(struct GameBoard*, int*);
void AIpositionSelection(struct GameBoard*, int*);
void recordGameInFile(struct GameBoard*, struct Deque*);

// TIC TAC TOE / CONNECT 4 
int main( int argc, const char* argv[])
{
	printf("\nWelcome to TIC TAC TOE/CONNECT 4.\n\n");

	MainMenu();

	// Game board that is going to be used during all the game
	struct GameBoard gameBoard;
	// Deque used to store the movements of the players
	struct Deque deque;
	// Stack used for undo functionality
	struct Stack undoStack;
	// Stack used for redo functionality
	struct Stack redoStack;

	// Load the game elements
	Initialise(&gameBoard);
	Draw(&gameBoard);

	// Initialises the deque and the stacks to store the game moves based on the gameboard grid size
	initialiseDeque(&deque, gameBoard.rows * gameBoard.columns);
	initialiseStack(&undoStack, gameBoard.rows * gameBoard.columns);
	initialiseStack(&redoStack, gameBoard.rows * gameBoard.columns);

	// Game Loop
	bool player1Turn = true;
	while (1)
	{
		Update(&gameBoard, &deque, &undoStack, &redoStack, player1Turn);
		Draw(&gameBoard);
		GameOver(&gameBoard, &deque, &undoStack, &redoStack, &player1Turn);
		player1Turn = !player1Turn;
	}

	return 0;
}

// "Main menu" of the game
void MainMenu()
{
	while (1)
	{
		printf("Enter 1 to start a new game | Enter 2 to watch a previous game.\n");
		int selection;
		scanf("%d", &selection);
		if (selection == 1)
		{
			return;
		}
		else if (selection == 2)
		{
			break;
		}
		// Clear stdin buffer to avoid infinite loop if string is entered
		fseek(stdin, 0, SEEK_END);
	}
	RecordingsMenu();
}

// Handles displaying the recordings
void RecordingsMenu()
{
	FILE* file = fopen("GameRecordings.txt", "r");
	// Check if ile is empty
	if (file == NULL)
	{
		printf("\nThere are no recordings stored, starting game.\n");
		return;
	}
	int fsize = 0;
	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	if (fsize == 0)
	{
		printf("\nThere are no recordings stored, starting game.\n");
		return;
	}
	// Go back to the beginning of the file
	rewind(file);

	// Figure the number of recordings out
	int numberOfRecordings = 0;
	while (!feof(file))
	{
		char ch;
		ch = fgetc(file);
		if (ch == '\n')
		{
			numberOfRecordings++;
		}
	}
	// Go back to the beginning of the file
	rewind(file);

	// Allocate memory for all the recordings
	struct Record* gamesRecordings = malloc(numberOfRecordings * sizeof(struct Record));
	// Read the file and create the records
	for(int i = 0; i < numberOfRecordings; i++)
	{
		// Temporal buffers to store the elements of a record
		int gameMode;
		char player1[256];
		char piece1;
		char player2[256];
		char piece2;
		char description[256];
		char moves[256];
		// First element is game type, the next four elements are player1 piece1 player2 piece2 and the sixth element is the description
		fscanf(file, "%d %s %c %s %c %s ", &gameMode, player1, &piece1, player2, &piece2, description);
		// The last elements until reaching the end of line are the sequence of moves
		char ch;
		do
		{
			ch = fgetc(file);
			strncat(moves, &ch, 1);

		} while (ch != '\n');
		// Create record and store it in the gameRecordings array
		createRecord(&gamesRecordings[i], gameMode, player1, piece1, player2, piece2, description, moves);
		memset(moves, 0, sizeof moves);
	}

	// Display recordings
	for (int i = 0; i < numberOfRecordings; i++)
	{
		// Rows and columns depend on the game mpde
		int rows;
		int columns;
		if (gamesRecordings[i].gameMode == 3)
		{
			rows = 3;
			columns = 3;
		}
		else
		{
			rows = 6;
			columns = 7;
		}
		// Create gameboard for the current record (for the records the AI does not matter)
		struct GameBoard currentGame;
		setUpGame(&currentGame, gamesRecordings[i].gameMode, columns, rows, gamesRecordings[i].player1, gamesRecordings[i].player2, gamesRecordings[i].piece1, gamesRecordings[i].piece2, false);

		// Present record to player
		printf("\n%s %s:%c %s:%c\n", gamesRecordings[i].gameDescription, gamesRecordings[i].player1, gamesRecordings[i].piece1, gamesRecordings[i].player2, gamesRecordings[i].piece2);
		// Draw the record
		Draw(&currentGame);

		// X player always goes first
		bool player1Turn = true;
		// Display the moves
		while (!emptyDeque(&gamesRecordings[i].moves))
		{
			// Decide what to do next
			int selection = 0;
			while (1)
			{
				printf("\nEnter 1 for next move | Enter 2 to exit\n");
				scanf("%d", &selection);
				if (selection == 1 || selection == 2)
				{
					break;
				}
				// Clear stdin buffer to avoid infinite loop if string is entered
				fseek(stdin, 0, SEEK_END);
			}
			// The update is done automatically without user input
			int position;
			char piece;
			// Player 1 and 2 take turns, starting player1
			if (selection == 1)
			{
				// Position to place the piece
				position = pop_front(&gamesRecordings[i].moves);
				// Piece
				if (player1Turn)
				{
					piece = gamesRecordings[i].piece1;
					player1Turn = false;
				}
				else
				{
					piece = gamesRecordings[i].piece2;
					player1Turn = true;
				}
				// Update the grid
				updateGrid(&currentGame, piece, position);
			}
			else if (selection == 2)
			{
				break;
			}
			// Draw the record
			printf("\n%s %s:%c %s:%c\n", gamesRecordings[i].gameDescription, gamesRecordings[i].player1, gamesRecordings[i].piece1, gamesRecordings[i].player2, gamesRecordings[i].piece2);
			printf("\nPiece %c place in position %d\n", piece, position);
			Draw(&currentGame);
		}

		// Free memory used for the game board
		freeGameBoard(&currentGame);
		// Free memory used for the current record
		freeRecord(&gamesRecordings[i]);

		// Next action
		printf("\nRecording finished.\n");
		int selection = 0;
		while (1)
		{
			printf("\nEnter 1 to watch next recording | Enter 2 to start the game.\n");
			scanf("%d", &selection);
			if (selection == 1)
			{
				break;
			}
			else if (selection == 2)
			{
				return;
			}
			// Clear stdin buffer to avoid infinite loop if string is entered
			fseek(stdin, 0, SEEK_END);
		}
	}
	// Free memory used for the records
	free(gamesRecordings);
	printf("\nNo more recordings available. Starting game.\n");
}


// Initialises the game
void Initialise(struct GameBoard* gameBoard)
{
	printf("\n");
	printf("Please enter your preference settings to start playing.\n");
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
		// Clear stdin buffer to avoid infinite loop if string is entered
		fseek(stdin, 0, SEEK_END);
	}
	printf("-------------------------------------------------------------------------------------\n");

	// In tic tac toe allow the player to select an AI to play with
	bool AI = false;
	if (winCount == 3)
	{
		while (1)
		{
			printf("Enter 1 to play against a second player | Enter 2 to play against an AI\n");
			int selection;
			scanf("%d", &selection);
			if (selection == 1)
			{
				break;
			}
			else if (selection == 2)
			{
				AI = true;
				break;
			}
			// Clear stdin buffer to avoid infinite loop if string is entered
			fseek(stdin, 0, SEEK_END);
		}
		printf("-------------------------------------------------------------------------------------\n");
	}

	// Player 1 elements
	char namePlayer1[256];
	char piecePlayer1;
	// Player 2 elements
	char namePlayer2[256];
	char piecePlayer2;
	// Enter Players names
	printf("Enter player 1 name: ");
	scanf("%s", namePlayer1);
	if (!AI)
	{
		printf("Enter player 2 name: ");
		scanf("%s", namePlayer2);
	}
	else
	{
		strcpy(namePlayer2, "AI");
	}
	printf("-------------------------------------------------------------------------------------\n");

	// Select pieces
	while (1)
	{
		printf("Select player 1 piece: Enter X for crosses | Enter O for noughts\n");
		char selection;
		scanf(" %c", &selection);
		if (selection == 'X' || selection == 'x')
		{
			piecePlayer1 = 'X';
			piecePlayer2 = 'O';
			break;
		}
		else if (selection == 'O' || selection == 'o')
		{
			piecePlayer1 = 'O';
			piecePlayer2 = 'X';
			break;
		}
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
	setUpGame(gameBoard, winCount, columns, rows, namePlayer1, namePlayer2, piecePlayer1, piecePlayer2, AI);
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
void GameOver(struct GameBoard* gameBoard, struct Deque* deque, struct Stack* undoStack, struct Stack* redoStack, bool* player1Turn)
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
		bool gameRecorded = false;
		while (1)
		{
			printf("Enter 1 to play again | Enter 2 to record the game | Enter 3 to exit the game\n");
			int selection = 0;
			scanf("%d", &selection);
			if (selection == 1)
			{
				// Restart gameboard
				playAgain(gameBoard);
				// Reset queue
				resetDeque(deque);
				// Reset undo stack
				resetStack(undoStack);
				// Reset redo stack
				resetStack(redoStack);
				// Draw the initial game board again
				Draw(gameBoard);
				// Reset player
				*player1Turn = false;
				break;
			}
			else if (selection == 2)
			{
				if (!gameRecorded)
				{
					recordGameInFile(gameBoard, deque);
					gameRecorded = true;
				}
				else
				{
					printf("Game already recorded\n");
				}
			}
			else if (selection == 3)
			{
				exit(0);
			}
			// Clear stdin buffer to avoid infinite loop if string was entered
			fseek(stdin, 0, SEEK_END);
		}
	}
}

void Update(struct GameBoard* gameBoard, struct Deque* deque, struct Stack* undoStack, struct Stack* redoStack, bool player1Turn)
{
	// Player 1 is 0, Player 2 is 1
	int i;
	if (player1Turn)
	{
		i = 0;
	}
	else
	{
		i = 1;
	}

	// Call sub update based on having AI or not
	if (gameBoard->AI)
	{
		AIUpdate(gameBoard, deque, i);
	}
	else
	{
		NormalUpdate(gameBoard, deque, undoStack, redoStack, i);
	}

	// Check for winner
	winnerCheck(gameBoard);
	// The winner is the player that performed the last move
	if (gameBoard->gameOver)
	{
		gameBoard->winnerName = gameBoard->players[i].name;
		return;
	}

	// Check for no longer positions available
	availablePosCheck(gameBoard);
	if (gameBoard->gameOver)
	{
		return;
	}
}

void NormalUpdate(struct GameBoard* gameBoard, struct Deque* deque, struct Stack* undoStack, struct Stack* redoStack, int i)
{
	// Undo or redo previous action
	while (1)
	{
		printf("Enter 1 to continue | Enter 2 to undo previous movement | Enter 3 to redo previous movement\n");
		int selection = 0;
		scanf("%d", &selection);
		if (selection == 1)
		{
			break;
		}
		else if (selection == 2)
		{
			if (emptyStack(undoStack))
			{
				printf("No movements to undo\n");
				continue;
			}
			int undoPosition;
			undoPosition = stackPop(undoStack);
			// Add element to redo stack
			stackPush(redoStack, undoPosition);
			// Update the grid with '\0'
			char undoCh = '\0';
			updateGrid(gameBoard, undoCh, undoPosition);
			// Remove element of the deque from the back
			pop_back(deque);
			return;
		}
		else if (selection == 3)
		{
			if (emptyStack(redoStack))
			{
				printf("No movements to redo\n");
				continue;
			}
			int redoPosition;
			redoPosition = stackPop(redoStack);
			// Update the grid based on previous players turn
			updateGrid(gameBoard, gameBoard->players[i].piece, redoPosition);
			// Add position to deque
			push_back(deque, redoPosition);
			// Add element to undo stack
			stackPush(undoStack, redoPosition);
			return;
		}
		// Clear stdin buffer to avoid infinite loop if string was entered
		fseek(stdin, 0, SEEK_END);
	}
	// Ask player to select position to place its piece
	printf("%s select position to place %c : ", gameBoard->players[i].name, gameBoard->players[i].piece);
	// Make sure that position enter by player is valid
	int position = 0;
	positionSelection(gameBoard, &position);
	// Add position to deque
	push_back(deque, position);
	// Add position to the stack
	stackPush(undoStack, position);
	// Empty redo stack
	resetStack(redoStack);
	// Update grid
	updateGrid(gameBoard, gameBoard->players[i].piece, position);
}

void AIUpdate(struct GameBoard* gameBoard, struct Deque* deque, int i)
{
	int position = 0;
	if (gameBoard->players[i].AI)
	{
		printf("AI selecting position.\n");
		// AI selects a position to place its piece
		AIpositionSelection(gameBoard, &position);
	}
	else
	{
		// Ask player to select position to place its piece
		printf("%s select position to place %c : ", gameBoard->players[i].name, gameBoard->players[i].piece);
		// Make sure that position enter by player is valid
		positionSelection(gameBoard, &position);
	}
	// Add position to deque
	push_back(deque, position);
	// Update grid
	updateGrid(gameBoard, gameBoard->players[i].piece, position);
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

// Function that selects a random position in the grid for the AI until it picks one available
void AIpositionSelection(struct GameBoard* gameBoard, int* position)
{
	while (1)
	{
		srand(time(0));
		*position = (rand() % ((gameBoard->columns * gameBoard->rows))) + 1;
		// Map the position entered to grid location
		int x = (*position - 1) / gameBoard->columns;
		int y = (*position - 1) % gameBoard->columns;
		// Position out of range or already taken
		if (*position <= 0 || *position > (gameBoard->columns * gameBoard->rows) || gameBoard->grid[x][y] != '\0')
		{
			continue;
		}
		else
		{
			break;
		}
	}
}

// Record the game in a file
void recordGameInFile(struct GameBoard* gameBoard, struct Deque* deque)
{
	FILE* gameRecordings = fopen("GameRecordings.txt", "a");
	// The first element to print is the win count which will be used to determine the game type 
	fprintf(gameRecordings, "%d ", gameBoard->winCount);
	// The next element are the player names with their corresponding pieces
	fprintf(gameRecordings, "%s %c %s %c ", gameBoard->players[0].name, gameBoard->players[0].piece, gameBoard->players[1].name, gameBoard->players[1].piece);
	// The next element is a description for the game
	printf("Enter a short description for this match, for example: Match1WonByJoe. Everything after the first space will be ignored.");
	char description[256];
	fseek(stdin, 0, SEEK_END);
	scanf("%s", description);
	fprintf(gameRecordings, "%s ", description);
	// The final elements to store in the file are the positions played during the game
	while (!emptyDeque(deque))
	{
		int position = pop_front(deque);
		fprintf(gameRecordings, "%d ", position);
	}
	fprintf(gameRecordings, "\n");
	// Close file
	fclose(gameRecordings);
}
