//
// Created by chamod on 1/23/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include "timer.h"

struct node {
    int data;
    struct node *next;
};

//const int MAX_THREADS = 1024;
long thread_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct node *head = NULL;

int n, m;
float m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete;

int Member(int value, struct node *head) {
    struct node *current = head;
    while (current != NULL && current->data < value)
        current = current->next;

    if (current == NULL || current->data > value) {
        return 0;
    } else {
        return 1;
    }

}


int Insert(int value, struct node **head) {
    struct node *current = *head;
    struct node *pred = NULL;
    struct node *temp;
    while (current != NULL && current->data < value) {
        pred = current;
        current = current->next;
    }
    if (current == NULL || current->data > value) {
        temp = malloc(sizeof(struct node));
        temp->data = value;
        temp->next = current;
        if (pred == NULL)
            *head = temp;
        else
            pred->next = temp;
        return 1;
    } else {
        return 0;
    }
}

int Delete(int value, struct node **head) {
    struct node *current = *head;
    struct node *pred = NULL;
    while (current != NULL && current->data < value) {
        pred = current;
        current = current->next;
    }
    if (current != NULL && current->data == value) {
        if (pred == NULL) {
            *head = current->next;
            free(current);
        } else {
            pred->next = current->next;
            free(current);
        }
        return 1;
    } else {
        return 0;
    }
}

void GetArguments(int argc, char *argv[]) {

    thread_count = strtol(argv[1], NULL, 10);
    n = (int) strtol(argv[2], (char **) NULL, 10);
    m = (int) strtol(argv[3], (char **) NULL, 10);

    m_percentage_of_Member = (float) atof(argv[3]);
    m_percentage_of_Insert = (float) atof(argv[4]);
    m_percentage_of_Delete = (float) atof(argv[5]);
}

double CalculateTime(double start, double end) {
    return (end - start);
}

void Display(struct node **head_pp) {
    struct node *r = *head_pp;
    if (r == NULL) {
        return;
    }
    while (r != NULL) {
        printf("%d ", r->data);
        r = r->next;
    }
    printf("\n");
}

void *pthread_on_linked_list(void *rank) {

    long long i;
    long long my_m = m / thread_count;

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

    return CalculateTime(start, end);

}

int main(int argc, char *argv[]) {
    int samples = 10;

    for (int j = 0; j < samples; j++) {
        pthread_mutex_init(&mutex, NULL);
        float time = linkedList_mutex_program(argc, argv);
        pthread_mutex_destroy(&mutex);
        printf("%.6f  \n", time);
    }
    return 0;

}