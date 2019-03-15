#ifndef STORAGE
#define STORAGE

#include <string.h>
#include <stdbool.h>
struct Deque
{
	int* arr;	// Array for the underlying implementation of the deque
	int max;	// Total number of elements in the queue
	int front;	// Head of the queue
	int rear;	// Tail of the queue
};
struct Stack
{
	int* arr;	// Array for the underlying implementation of the stack
	int max;	// Total number of elements in the stack
	int top;	// Top part of the stack
};
struct Record
{
	char* player1;			// Player 1 name
	char* player2;			// Player 2 name
	char piece1;			// Piece of player 1, X or O
	char piece2;			// Piece of player 2, X or O
	char* gameDescription;	// Game Description
	int gameMode;			// 3 for tic tac toe, 4 for connect 4
	struct Deque moves;		// Deque that is going to store the moves
};

// Queue API

// Initialise the deque
void initialiseDeque(struct Deque* queue, int max);
// Add element to the back of the deque
void push_back(struct Deque* queue, int item);
// Return the element at the front of the deque
int pop_front(struct Deque* queue);
// Checks wheter the deque is empty or not
bool emptyDeque(struct Deque* queue);
// Empties the deque
void resetDeque(struct Deque* queue);
// Removes the element at the tail of the deque
int pop_back(struct Deque* queue);

// Stack API

// Initialises the stack
void initialiseStack(struct Stack* stack, int max);
// Add elements to stack
void stackPush(struct Stack* stack, int item);
// Return the element at the top of the stack
int stackPop(struct Stack* stack);
// Checks wheter the stack is empty or not
bool emptyStack(struct Stack* stack);
// Empties the stack
void resetStack(struct Stack* stack);


// Record API

// Creates record
void createRecord(struct Record* record, int gameMode, char* player1, char piece1, char* player2, char piece2, char* description, char* moves);
// Frees memory used by record
void freeRecord(struct Record* record);

#endif