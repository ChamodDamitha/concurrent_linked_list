#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include "timer.h"
#include "linkedList.h"

int n, m;
float m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete;

long i;
long my_m;

void GetArguments(int argc, char *argv[]) {

    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);

    m_percentage_of_Member = (float) atof(argv[3]);
    m_percentage_of_Insert = (float) atof(argv[4]);
    m_percentage_of_Delete = (float) atof(argv[5]);
}

float linked_list_serial_program(int argc, char *argv[]) {
    double start, end;

    struct node *head = NULL;
    int i;

    GetArguments(argc, argv);

    for (; i < n; i++) {
        int r = rand() % 65536;
        if (!Insert(r, &head)) {
            i--;
        }
    }

    GET_TIME(start);
    for (i = 0; i < m; i++) {
        float temp = (rand() % 10000 / 10000.0);
        int r = rand() % 65536;

        if (temp < m_percentage_of_Member) {
            Member(r, head);
        } else if (temp < m_percentage_of_Member + m_percentage_of_Insert) {
            Insert(r, &head);
        } else {
            Delete(r, &head);
        }
    }

    GET_TIME(end);

    return end - start;
}


int main(int argc, char *argv[]) {
    const int samples = 3;

    for (int j = 0; j < samples; j++) {
        float time = linked_list_serial_program(argc, argv);
        printf("%.6f\n", time);
    }

    return 0;
}