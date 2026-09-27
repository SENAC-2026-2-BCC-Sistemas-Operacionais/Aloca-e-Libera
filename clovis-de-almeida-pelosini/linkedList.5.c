#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct DoublyLinkedList {
    Node *head;
    Node *tail;
    size_t size;
} DoublyLinkedList;

DoublyLinkedList* list_create() {
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    if (list != NULL) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

int list_append(DoublyLinkedList *list, void *data) {
    if (list == NULL) return -1;

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return -1;

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return 0;
}

int list_prepend(DoublyLinkedList *list, void *data) {
    if (list == NULL) return -1;

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return -1;

    new_node->data = data;
    new_node->next = list->head;
    new_node->prev = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->head->prev = new_node;
        list->head = new_node;
    }

    list->size++;
    return 0;
}

void list_free(DoublyLinkedList *list) {
    if (list == NULL) return;

    Node *current = list->head;
    while (current != NULL) {
        Node *next_node = current->next;
        free(current);
        current = next_node;
    }
    free(list);
}
