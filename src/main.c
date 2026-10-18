#include "codexion.h"
#include <getopt.h>
#include <stdlib.h>
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
        _ft_putstr("\033[41m  MISSING ARGUMENT  \033[49m\n");
        _ft_putstr("\tmissing requirements arguments\n");
        _ft_putstr("\tusage: ./codexion nc<int> tb<int> tc<int> td<int> tr<int> ncr<int> dc<int> s<str>\n");
    }
}

void print_help(const char *prog_name) {
    printf("\nUsage: %s [options | arguments]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help    Show this help message and exit\n");
    printf("Arguments:\n");
    printf("  number_of_coders\n");
    printf("  time_to_burnout\n");
    printf("  time_to_compile\n");

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

    int opt;

    while ((opt = getopt_long(argc, argv, "h", log_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
                break;
        
        }
    }



    if (argc == 9)
        printf("%s\n", argv[0]);
    else if (argc == 2)
        printf("hehehe");
    else {
        _display_error(MISSING_ARGUMENT);
        exit(1);
    }

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
