#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};


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

