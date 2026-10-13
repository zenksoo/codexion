#include "codexion.h"
#include <pthread.h>
#include <sys/_pthread/_pthread_t.h>
#include <unistd.h>


void *t_func(void *p) {
    while (1) {
        printf("i'm the first thread created %d\n", *(int *)p);
        sleep(1);
    }
    return NULL;
}


int main() {
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
