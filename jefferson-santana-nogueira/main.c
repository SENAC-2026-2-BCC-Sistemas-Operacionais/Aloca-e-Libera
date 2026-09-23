#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void insert_front(Node** head, int data) {
    Node* new_node = create_node(data);
    new_node->next = *head;
    
    if (*head != NULL) {
        (*head)->prev = new_node;
    }
    
    *head = new_node;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* head = NULL;
    insert_front(&head, 10);
    insert_front(&head, 20);
    insert_front(&head, 30);

    print_list(head);
    
    free_list(head);

    return 0;
}
