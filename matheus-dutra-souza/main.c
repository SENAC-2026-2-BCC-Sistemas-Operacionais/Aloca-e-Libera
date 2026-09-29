#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define POOL_SIZE 16384

char memory_pool[POOL_SIZE];

typedef struct Block {
    size_t size;         
    int is_free;         
    struct Block* next;  
} Block;

Block* free_list = (void*)memory_pool;
int initialized = 0;

void init_pool() {
    free_list->size = POOL_SIZE - sizeof(Block); 
    free_list->is_free = 1;
    free_list->next = NULL;
    initialized = 1;
}

void* aloca(size_t size) {
    if (!initialized) {
        init_pool();
    }

    Block* curr = free_list;
    
    while (curr != NULL) {
        if (curr->is_free && curr->size >= size) {

            if (curr->size > size + sizeof(Block)) {
                
                Block* new_block = (Block*)((char*)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->is_free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }
            
            curr->is_free = 0; 
            
           
            return (void*)(curr + 1); 
        }
        curr = curr->next;
    }
    
    printf("erro, memoria insuficiente no buffer.\n");
    return NULL; 
}


void merge_free_blocks() {
    Block* curr = free_list;
    while (curr != NULL && curr->next != NULL) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}


void libera(void* ptr) {
    if (ptr == NULL) return;
    
    Block* block = (Block*)ptr - 1; 
    block->is_free = 1;
    
    merge_free_blocks(); 
}


typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

Node* insertFront(Node* head, int value) {
   
    Node* newNode = (Node*)aloca(sizeof(Node)); //usar aloca
    if (newNode == NULL) return head;

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
        libera(temp); // usar o libera
    }
    printf("memoria liberada\n");
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