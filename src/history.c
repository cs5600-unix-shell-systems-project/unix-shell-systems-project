#include "history.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// vector and methods for storing strings:

int init_vector(Vector *v) {

    v->length = 0;
    v->capacity = 4;
    v->commands = malloc(v->capacity * sizeof *v->commands);

    if (!v->commands) {
        v->capacity = 0;
        return 0;
    }

    return 1;
}

int vector_push(Vector *v, char *command) {
    //hit capacity in the command array, double the size
    if (v->length == v->capacity) {

        size_t new_capacity = v->capacity * 2;
        size_t bytes = new_capacity * sizeof *v->commands;

        char **new_commands = realloc(v->commands, bytes);
        if (!new_commands) {
            return 0;
        } else {
            v->commands = new_commands;
            v->capacity = new_capacity;
        }
    }

    //get the length of the string
    size_t length = strlen(command);

    //malloc room for the string and null terminator
    char *copy = malloc(length + 1);

    if (!copy) {
        return 0;
    } else {
        memcpy(copy, command, length + 1);
        v->commands[v->length] = copy;
        v->length++;
        return 1;
    }
}

/*int vector_free(Vector *v) {

}*/

//write line
void print_history(Vector *v) {
    for (size_t i = 0; i < v->length; i++) {
        printf("%zu: %s\n", i, v->commands[i]);
    }
}
