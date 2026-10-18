#include "codexion.h"
#include <asm-generic/errno-base.h>
#include <stdbool.h>
#include <threads.h>
#include <stdio.h>


typedef enum {
    INVALID_ARGUMENT
} Errors;

void *t_func(void *p) {
    while (1) {
        printf("i'm the first thread created %d\n", *(int *)p);
        sleep(1);
    }
    return NULL;
}


int ft_atoi(char *target) {
    unsigned int num = 0;
    int sign = 1;

    while (*target == '\t' || *target == ' ')
        target++;
    while (*target == '-' || *target == '+') {
        if (*target == '-') {
            sign *= -1;
        }
        target++;
    }

    while (*target <= '9' && *target >= '0') {
        num = (num * 10) + (*target - '0');
        target++;
    }
    return (int)(num * sign);
}


void _display_error(Errors status) {
    if (status == 0)
        printf("invalid argument error\n");

}


int main(int argc, char *argv[]) {
    // int number_of_coders;
    // int time_to_burnout;
    // int time_to_compile;
    // int time_to_debug;
    // int time_to_refactor;
    // int number_of_compiles_required;
    // int dongle_cooldown;
    // int scheduler;



    if (argc == 9)
        printf("%s\n", argv[0]);
    else
        _display_error(INVALID_ARGUMENT);

    printf("%d\n", ft_atoi("       --2147483647"));

    void *ptr;
    int n = 200;
    int n2 = 300;
    pthread_t id;
    pthread_t second_id;
    pthread_create(&id, NULL, (void *(*)(void *))t_func, (void *)&n);
    pthread_create(&second_id, NULL, (void *(*)(void *))t_func, (void *)&n2);
    pthread_join(second_id, &ptr);
    return 0;
}
