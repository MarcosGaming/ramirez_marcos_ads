#include "storage.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// QUEUE

// Initialise the queue
void initialiseQueue(struct Queue* queue, int max)
{
	queue->front = -1;
	queue->rear = -1;
	queue->max = max;
	// Allocate memory for the queue
	queue->arr = (int*)malloc(max * sizeof(int));
}
// Add element to the back of the queue
void enqueue(struct Queue* queue, int item)
{
	// The end of the array is reached
	if (queue->rear == queue->max - 1)
	{
		return;
	}
	else
	{
		// Add item to the tail of the queue
		queue->rear++;
		queue->arr[queue->rear] = item;
		// Queue is no longer empty
		if (queue->front == -1)
		{
			queue->front = 0;
		}
	}
}
// Return the element at the front of the queue
int dequeue(struct Queue* queue)
{
	// Get the element at the front
	int item = queue->arr[queue->front];
	// Replace the element at the front with 0
	queue->arr[queue->front] = 0;
	// Front has reached the tail of the queue
	if (queue->front == queue->rear)
	{
		queue->front = -1;
		queue->rear = -1;
	}
	else
	{
		queue->front++;
	}
	return item;
}
// Returns true if the queue is empty
bool emptyQueue(struct Queue* queue)
{
	if (queue->front == -1)
	{
		return true;
	}
	return false;
}

// RECORD

// Creates record
void createRecord(struct Record* record, int gameMode, char* player1, char piece1, char* player2, char piece2, char* description, char* moves)
{
	// Set game mode
	record->gameMode = gameMode;

	// Get players array sizes
	int player1Size = 0;
	int player2Size = 0;
	for (int i = 0; player1[i] != '\0'; i++)
	{
		player1Size++;
	}
	for (int i = 0; player2[i] != '\0'; i++)
	{
		player2Size++;
	}
	// Add one more to the size for null terminator
	player1Size++;
	player2Size++;
	// Allocate memory for players
	record->player1 = (char*)malloc(player1Size * sizeof(char));
	record->player2 = (char*)malloc(player2Size * sizeof(char));
	// Set the players
	strncpy(record->player1, player1, player1Size);
	strncpy(record->player2, player2, player2Size);

	// Set pieces
	record->piece1 = piece1;
	record->piece2 = piece2;

	// Get description array size
	int descriptionSize = 0;
	for (int i = 0; description[i] != '\0'; i++)
	{
		descriptionSize++;
	}
	// Add one more to the size for null terminator
	descriptionSize++;
	// Allocate memory for the description
	record->gameDescription = (char*)malloc(descriptionSize * sizeof(char));
	// Set game description
	strncpy(record->gameDescription, description, descriptionSize);

	// Get the amount of moves
	int movesNumber = 0;
	for (int i = 0; moves[i] != '\0'; i++)
	{
		if (isdigit(moves[i]) && !isdigit(i + 1))
		{
			movesNumber++;
		}
	}
	// Initialise record queue based on the number of moves
	initialiseQueue(&record->moves, movesNumber);
	// The moves are not going to have more than two digits
	char number[2];
	// Add the moves to the queue
	for (int i = 0; moves[i] != '\0'; i++)
	{
		if (isdigit(moves[i]) && isdigit(moves[i + 1]))
		{
			number[0] = moves[i];
			number[1] = moves[i + 1];
			enqueue(&record->moves, atoi(number));
		}
		else if (isdigit(moves[i]) && isdigit(moves[i - 1]))
		{
			//Already stored
			continue;
		}
		else if (isdigit(moves[i]) && !isdigit(moves[i + 1]))
		{
			enqueue(&record->moves, moves[i] - '0');
		}
	}
}

// Frees memory used by record
void freeRecord(struct Record* record)
{
	free(record->player1);
	free(record->player2);
	free(record->gameDescription);
	free(record->moves.arr);
}