#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include "timer.h"
#include "linkedList.h"


long thread_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct node *head = NULL;

int n, m;
float m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete;

long i;
long my_m;


void GetArguments(int argc, char *argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    n = (int) strtol(argv[2], (char **) NULL, 10);
    m = (int) strtol(argv[3], (char **) NULL, 10);

    m_percentage_of_Member = (float) atof(argv[3]);
    m_percentage_of_Insert = (float) atof(argv[4]);
    m_percentage_of_Delete = (float) atof(argv[5]);
}

void *pthread_on_linked_list(void *rank) {

    my_m = m / thread_count;

    for (i = 0; i < my_m; i++) {
        float prob = (rand() % 10000 / 10000.0);

        int r = rand() % 65536;

        if (prob < m_percentage_of_Member) {
            pthread_mutex_lock(&mutex);
            Member(r, head);
            pthread_mutex_unlock(&mutex);

        } else if (prob < m_percentage_of_Member + m_percentage_of_Insert) {
            pthread_mutex_lock(&mutex);
            Insert(r, &head);
            pthread_mutex_unlock(&mutex);

        } else {
            pthread_mutex_lock(&mutex);
            Delete(r, &head);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}


float linkedList_mutex_program(int argc, char *argv[]) {
    int i = 0;
    long thread;
    pthread_t *thread_handles;
    double_t start, end;
    GetArguments(argc, argv);

    head = NULL;

    for (; i < n; i++) {
        int r = rand() % 65536;
        if (!Insert(r, &head)) {
            i--;
        }
    }

    thread_handles = (pthread_t *) malloc(thread_count * sizeof(pthread_t));


    GET_TIME(start);

    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, pthread_on_linked_list, (void *) thread);
    }

    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    GET_TIME(end);

    return (end - start);

}

int main(int argc, char *argv[]) {
    const int samples = 54;

    for (int j = 0; j < samples; j++) {
        pthread_mutex_init(&mutex, NULL);
        float time = linkedList_mutex_program(argc, argv);
        pthread_mutex_destroy(&mutex);
        printf("%.6f  \n", time);
    }
    return 0;
}