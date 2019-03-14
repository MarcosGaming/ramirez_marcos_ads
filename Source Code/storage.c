#include "storage.h"
#include <stdlib.h>
#include <stdio.h>

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