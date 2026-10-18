#ifndef CODEXION_H
#define CODEXION_H

#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// remote at the end
#include <stdio.h>

typedef enum {
    MISSING_ARGUMENT,
    INVALID_ARGUMENT,
} Errors;

typedef struct Arguments {
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
} args;

#endif
