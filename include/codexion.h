#ifndef CODEXION_H
#define CODEXION_H

#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

// remote at the end 
#include <stdio.h>

typedef enum {
    MISSING_ARGUMENT,
    INVALID_ARGUMENT,
} Errors;

int ft_atoi(char *target);
void _ft_putstr(char *str);
void _print_help();

struct option log_options[] = {
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

#endif
