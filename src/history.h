// I need to setup a vector and a vector push method, need to worry about clearing this on crash and shell closure

// I need to capture up and down arrow key presses, this will increment or decrement a counter to make use of
// direct access on the vector

// I need to edit the most recent line in the shell

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    char **commands;
    size_t length;
    size_t capacity;
} Vector;

int init_vector(Vector *v);
int vector_push(Vector *v, char *command);
//void vector_free(Vector *v);

#endif