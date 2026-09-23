#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

Node* insertFront(Node* head, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = head;

    if (head != NULL) {
        head->prev = newNode;
    }
    return newNode;
}

void printList(Node* node) {
    printf("Lista: ");
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp); // Liberação de memória
    }
    printf("Memoria liberada com sucesso.\n");
}

int main() {
    Node* head = NULL;

    head = insertFront(head, 30);
    head = insertFront(head, 20);
    head = insertFront(head, 10);

    printList(head);

    freeList(head);

    return 0;
}
