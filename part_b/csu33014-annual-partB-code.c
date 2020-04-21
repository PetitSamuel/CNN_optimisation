//
// CSU33014 Summer 2020 Additional Assignment
// Part B of a two-part assignment
//
// Please write your solution in this file

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include "csu33014-annual-partB-person.h"

void find_reachable_recursive(struct person *current, int steps_remaining,
                              bool *reachable)
{
  // mark current root person as reachable
  reachable[person_get_index(current)] = true;
  // now deal with this person's acquaintances
  if (steps_remaining > 0)
  {
    int num_known = person_get_num_known(current);
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(current, i);
      find_reachable_recursive(acquaintance, steps_remaining - 1, reachable);
    }
  }
}

// computes the number of people within k degrees of the start person
int number_within_k_degrees(struct person *start, int total_people, int k)
{
  bool *reachable;
  int count;

  // maintain a boolean flag for each person indicating if they are visited
  reachable = malloc(sizeof(bool) * total_people);
  for (int i = 0; i < total_people; i++)
  {
    reachable[i] = false;
  }

  // now search for all people who are reachable with k steps
  find_reachable_recursive(start, k, reachable);

  // all visited people are marked reachable, so count them
  count = 0;
  for (int i = 0; i < total_people; i++)
  {
    if (reachable[i] == true)
    {
      count++;
    }
  }
  return count;
}

/*
  Custom implementation for a queue, required by my
  BFS algorithm.
  It is implemented using an array of persons with a fixed capacity.
*/
struct Queue
{
  int front, rear, size;
  unsigned capacity;
  struct person *array;
};

// Return pointer to a new empty queue with given capacity.
struct Queue *newQueue(unsigned capacity)
{
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
  queue->capacity = capacity;
  queue->size = 0;
  queue->front = 0;
  queue->rear = capacity - 1;
  queue->array = (struct person *)malloc(queue->capacity * sizeof(struct person));
  return queue;
}

// Helper method to check if provided queue is full (ie queue size == capacity)
int isFull(struct Queue *q)
{
  return (q->size == q->capacity);
}

// Helper method to check if provided queue is empty.
int isEmpty(struct Queue *q)
{
  return (q->size == 0);
}

// Add an item to a queue
void enqueue(struct Queue *q, struct person p)
{
  if (isFull(q))
  {
    printf("ERROR: TRIED TO ADD TO A QUEUE AT FULL CAPACITY\n");
    exit(1);
  }

  q->rear = (q->rear + 1) % q->capacity;
  q->array[q->rear] = p;
  q->size = q->size + 1;
}

// Remove an item from a queue
struct person dequeue(struct Queue *q)
{
  if (isEmpty(q))
  {
    printf("ERROR: TRIED TO ADD TO A QUEUE AT FULL CAPACITY\n");
    exit(1);
  }
  struct person p = q->array[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size = q->size - 1;
  return p;
}

// computes the number of people within k degrees of the start person;
// less repeated computation than the simple original version
int less_redundant_number_within_k_degrees(struct person *start,
                                           int total_people, int k)
{
  bool *visited = malloc(sizeof(bool) * total_people);
  // make sure all values are set to false
  for (int i = 0; i < total_people; i++)
  {
    visited[i] = false;
  }

  // allocate space for the queue, setting the max capacity to the total people - 1
  // when item is created, queue start person
  struct Queue *queue = newQueue(total_people - 1);
  int pi = person_get_index(start);

  // Set start person to visited and add start person to queue.
  visited[pi] = true;
  enqueue(queue, *start);

  // Define counter variable (ints)
  int count = 1;
  int currentDepth = 0,
      elementsToDepthIncrease = 1,
      nextElementsToDepthIncrease = 0;

  // Execute BFS
  while (!isEmpty(queue))
  {
    // obtain current person from queue
    struct person current = dequeue(queue);
    int num_known = person_get_num_known(&current);

    // Go through all acquaintances, setting them to visited & adding to queue if not visited previously
    // increment count on the same condition & if condition fails decrement the depthIncrease counter.
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(&current, i);
      pi = person_get_index(acquaintance);

      if (!visited[pi])
      {
        visited[pi] = true;
        count++;
        enqueue(queue, *acquaintance);
      }
      else
      {
        nextElementsToDepthIncrease--;
      }
    }
    /*
      Add number of aquaintances to nextDepthIncrease counter.
      If the following loop iteration will be of a new depth, increase currentDepth variable & 
      set counter variables for depth increments.
      If depth goes beyond k, return current count.
    */
    nextElementsToDepthIncrease += num_known;
    if (--elementsToDepthIncrease == 0)
    {
      if (++currentDepth >= k)
        return count;
      elementsToDepthIncrease = nextElementsToDepthIncrease;
      nextElementsToDepthIncrease = 0;
    }
  }

  return count;
}

// computes the number of people within k degrees of the start person;
// parallel version of the code
int parallel_number_within_k_degrees(struct person *start,
                                     int total_people, int k)
{
  bool *visited = malloc(sizeof(bool) * total_people);

  // make sure all values are set to false
  #pragma omp parallel for shared(visited) schedule(static, 1)
  for (int i = 0; i < total_people; i++)
  {
    visited[i] = false;
  }
  struct Queue *queue;
  #pragma omp parallel
  {
    // allocate space for the queue, setting the max capacity to the total people - 1
    // when item is created, queue start person
    #pragma omp task
    {
      queue = newQueue(total_people - 1);
      enqueue(queue, *start);
    }
    // set start person to visited
    #pragma omp task
    {
      visited[person_get_index(start)] = true;
    }
  }

  /*
    Define other variable (ints) outside tasks as they definitely don't require
    much work and would be required outside of the task scope anyways.
  */
  int count = 1;
  int currentDepth = 0,
      elementsToDepthIncrease = 1,
      nextElementsToDepthIncrease = 0;

  // Execute BFS
  while (!isEmpty(queue))
  {
    // obtain current person from queue
    struct person current = dequeue(queue);
    int num_known = person_get_num_known(&current);

    // Go through all acquaintances, setting them to visited & adding to queue if not visited previously
    // increment count on the same condition & if condition fails decrement the depthIncrease counter.
    #pragma omp parallel for shared(queue, count, nextElementsToDepthIncrease, visited) schedule(static, 1)
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(&current, i);
      int pi = person_get_index(acquaintance);

      if (!visited[pi])
      {
        #pragma omp critical(queue_item)
        {
          enqueue(queue, *acquaintance);
        }

        #pragma omp atomic
        count++;
        visited[pi] = true;
      }
      else
      {
        #pragma omp atomic
        nextElementsToDepthIncrease--;
      }
    }

    /*
      Add number of aquaintances to nextDepthIncrease counter.
      If the following loop iteration will be of a new depth, increase currentDepth variable &
      set counter variables for depth increments.
      If depth goes beyond k, return current count.
    */
    nextElementsToDepthIncrease += num_known;
    if (--elementsToDepthIncrease == 0)
    {
      if (++currentDepth >= k)
        return count;
      elementsToDepthIncrease = nextElementsToDepthIncrease;
      nextElementsToDepthIncrease = 0;
    }
  }

  return count;
}
