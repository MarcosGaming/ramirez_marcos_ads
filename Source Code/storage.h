#ifndef STORAGE
#define STORAGE

#include <string.h>
#include <stdbool.h>
struct Queue
{
	int* arr;	// Array for the underlying implementation of the queue
	int max;	// Total number of elements in the queue
	int front;	// Head of the queue
	int rear;	// Tail of the queue
};
struct Record
{
	char* player1;			// Player 1 name
	char* player2;			// Player 2 name
	char piece1;			// Piece of player 1, X or O
	char piece2;			// Piece of player 2, X or O
	char* gameDescription;	// Game Description
	int gameMode;			// 3 for tic tac toe, 4 for connect 4
	struct Queue moves;		// Queue that is going to store the moves
};

// Queue API

// Initialise the queue
void initialiseQueue(struct Queue* queue, int max);
// Add element to the back of the queue
void enqueue(struct Queue* queue, int item);
// Return the element at the front of the queue
int dequeue(struct Queue* queue);
// Checks wheter the queue is empty or not
bool emptyQueue(struct Queue* queue);

// Record API

// Creates record
void createRecord(struct Record* record, int gameMode, char* player1, char piece1, char* player2, char piece2, char* description, char* moves);
// Frees memory used by record
void freeRecord(struct Record* record);

#endif