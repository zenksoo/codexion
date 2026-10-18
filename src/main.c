#include "codexion.h"
#include <string.h>
#include <unistd.h>


void *t_func(void *p) {
    while (1) {
        printf("i'm the first thread created %d\n", *(int *)p);
        sleep(1);
    }
    return NULL;
}


void _display_error(Errors err_status) {
    if (err_status == MISSING_ARGUMENT) {
        printf("\033[41m  MISSING ARGUMENT  \033[49m\n");
        printf("    missing requirements arguments\n");
        printf("    usage:\n\t ./codexion ");
        printf("<number_of_coders>(int) <time_to_burnout>(int) <time_to_compile>(int) <time_to_debug>(int)");
        printf(" <time_to_refactor>(int) <number_of_compiles_required>(int) <dongle_cooldown>(int) <scheduler>(str)\n");
    }
}


int main(int argc, char *argv[]) {
    args *data;


    data = malloc(sizeof(args));


    if (argc == 9)
        printf("%s\n", argv[0]);
    else {
        _display_error(MISSING_ARGUMENT);
        return 1;
    }


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
