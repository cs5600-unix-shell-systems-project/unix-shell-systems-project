// History.h
// defines the vector/dynamic array that is used to store and recall
// previous user commands.
#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

typedef struct {
    char **commands;
    size_t length;
    size_t capacity;
} Vector;

// initialize the vector, length,
// capacity and mallocs the initial array
int init_vector(Vector *v);
// pushes a command into the array
int vector_push(Vector *v, char *command);
// frees the array
void vector_free(Vector *v);
// prints the historical command key mapping
void print_history(Vector *v);

#endif