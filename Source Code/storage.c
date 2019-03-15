#include "storage.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// DEQUE

// Initialise the deque
void initialiseDeque(struct Deque* deque, int max)
{
	deque->front = -1;
	deque->rear = -1;
	deque->max = max;
	// Allocate memory for the deque
	deque->arr = (int*)malloc(max * sizeof(int));
}
// Add element to the back of the deque
void push_back(struct Deque* deque, int item)
{
	// Deque empty
	if (deque->front == -1)
	{
		deque->front = 0;
		deque->rear = 0;
	}
	// The end of the array is reached
	else if (deque->rear == deque->max - 1)
	{
		deque->rear = 0;
	}
	else
	{
		deque->rear++;
	}
	// Add item to the tail of the deque
	deque->arr[deque->rear] = item;
}
// Return the element at the front of the deque
int pop_front(struct Deque* deque)
{
	// Get the element at the front
	int item = deque->arr[deque->front];
	// Replace the element at the front with -1
	deque->arr[deque->front] = -1;
	// Deque has only one element
	if (deque->front == deque->rear)
	{
		deque->front = -1;
		deque->rear = -1;
	}
	// Front has reached the end of the array
	else if (deque->front == deque->max - 1)
	{
		deque->front = 0;
	}
	else
	{
		deque->front++;
	}
	return item;
}
// Returns true if the deque is empty
bool emptyDeque(struct Deque* deque)
{
	if (deque->front == -1)
	{
		return true;
	}
	return false;
}
// Empties the deque
void resetDeque(struct Deque* deque)
{
	while (!emptyDeque(deque))
	{
		pop_front(deque);
	}
	deque->front = -1;
	deque->rear = -1;
}
// Removes the element at the tail of the deque
int pop_back(struct Deque* deque)
{
	// Get the element at the rear
	int item = deque->arr[deque->rear];
	// Replace the element at the rear with -1
	deque->arr[deque->rear] = -1;
	// Deque has only one element
	if (deque->front == deque->rear)
	{
		deque->front = -1;
		deque->rear = -1;
	}
	// Tail has reached the beginning
	else if (deque->rear == 0)
	{
		deque->rear = deque->max-1;
	}
	else
	{
		deque->rear--;
	}
	return item;
}

// STACK

// Initialises the stack
void initialiseStack(struct Stack* stack, int max)
{
	stack->top = -1;
	stack->max = max;
	// Allocate memory for the stack array
	stack->arr = (int*)malloc(max * sizeof(int));
}
// Add elements to stack
void stackPush(struct Stack* stack, int item)
{
	stack->top++;
	stack->arr[stack->top] = item;
}
// Return the element at the top of the stack
int stackPop(struct Stack* stack)
{
	int item = stack->arr[stack->top];
	stack->top--;
	return item;
}
// Checks wheter the stack is empty or not
bool emptyStack(struct Stack* stack)
{
	if (stack->top == -1)
	{
		return true;
	}
	return false;
}
// Empties the stack
void resetStack(struct Stack* stack)
{
	while (!emptyStack(stack))
	{
		stackPop(stack);
	}
	stack->top = -1;
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
	// Initialise record deque based on the number of moves
	initialiseDeque(&record->moves, movesNumber);
	// The moves are not going to have more than two digits
	char number[2];
	// Add the moves to the deque
	for (int i = 0; moves[i] != '\0'; i++)
	{
		if (isdigit(moves[i]) && isdigit(moves[i + 1]))
		{
			number[0] = moves[i];
			number[1] = moves[i + 1];
			push_back(&record->moves, atoi(number));
		}
		else if (isdigit(moves[i]) && isdigit(moves[i - 1]))
		{
			//Already stored
			continue;
		}
		else if (isdigit(moves[i]) && !isdigit(moves[i + 1]))
		{
			push_back(&record->moves, moves[i] - '0');
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