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

struct Queue
{
  int front, rear, size;
  unsigned capacity;
  struct iPerson *array;
};

struct Queue *createQueue(unsigned capacity)
{
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
  queue->capacity = capacity;
  queue->size = 0;
  queue->front = 0;
  queue->rear = capacity - 1;
  queue->array = (struct iPerson *)malloc(queue->capacity * sizeof(struct iPerson));
  return queue;
}

int isFull(struct Queue *queue)
{
  return (queue->size == queue->capacity);
}

int isEmpty(struct Queue *queue)
{
  return (queue->size == 0);
}

void enqueue(struct Queue *queue, struct iPerson item)
{
  if (isFull(queue))
  {
    // cannot add to a full queue
    printf("TRIED TO ADD TO A QUEUE AT FULL CAPACITY\n");
    exit(1);
  }

  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
struct iPerson dequeue(struct Queue *queue)
{
  if (isEmpty(queue))
  {
    printf("TRIED TO ADD TO A QUEUE AT FULL CAPACITY\n");
    exit(1);
  }
  struct iPerson item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}

/*
void less_redundant_find_reachable_recursive(struct person *current, int steps_remaining,
                                             bool *reachable)
{
  if (reachable[person_get_index(current)])
    return;

  // mark current root person as reachable
  reachable[person_get_index(current)] = true;
  // now deal with this person's acquaintances
  if (steps_remaining > 0)
  {
    int num_known = person_get_num_known(current);
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(current, i);
      if (steps_remaining > 1)
      {
        find_reachable_recursive(acquaintance, steps_remaining - 1, reachable);
      }
      else
      {
        reachable[person_get_index(acquaintance)] = true;
      }
    }
  }
}

// computes the number of people within k degrees of the start person;
// less repeated computation than the simple original version
int less_redundant_number_within_k_degrees(struct person *start,
                                           int total_people, int k)
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
  less_redundant_find_reachable_recursive(start, k, reachable);

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
*/
struct iPerson {
  struct person* person;
  int k;
};
struct iPerson createiPerson(int k, struct person* person)
{
  struct iPerson *iPerson = (struct iPerson *)malloc(sizeof(struct iPerson));
  iPerson->k = k;
  iPerson->person = person;
  return iPerson;
}

int getKFromiPerson(struct iPerson* person)
{
  return person->k;
}

struct person* getPersonFromiPerson(struct iPerson* person)
{
  return person->person;
}

// computes the number of people within k degrees of the start person;
// less repeated computation than the simple original version
int less_redundant_number_within_k_degrees(struct person *start,
                                           int total_people, int k)
{
  bool *visited = malloc(sizeof(bool) * total_people);
  for (int i = 0; i < total_people; i++)
  {
    visited[i] = false;
  }

  struct Queue *queue = createQueue(total_people);
  int curr = person_get_index(start);

  visited[curr] = true;
  struct iPerson a = createiPerson(k, start);
  enqueue(queue, a);
  int count = 1;
  while (!isEmpty(queue))
  {
    struct person current = dequeue(queue);
    int num_known = person_get_num_known(&current);
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(&current, i);
      curr = person_get_index(acquaintance);
      if (!visited[curr])
      {
        visited[curr] = true;
        count++;
        enqueue(queue, *acquaintance);
      }
    }
  }

  printf("%d", total_people);
  return count;
}

// computes the number of people within k degrees of the start person;
// parallel version of the code
int parallel_number_within_k_degrees(struct person *start,
                                     int total_people, int k)
{
  return number_within_k_degrees(start, total_people, k);
}
