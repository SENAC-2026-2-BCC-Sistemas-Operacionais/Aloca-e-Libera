#include <stdio.h>
#include <stdlib.h> 

typedef struct Node {
  int val;
  struct Node* prev;
  struct Node* next;
} Node;

Node* createNode(int value) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  
  newNode->val = value;
  newNode->prev = NULL;
  newNode->next = NULL;

  return newNode;
}

void insertStart(Node** listHead, int value) {
  Node* newNode = createNode(value);

  if (*listHead == NULL) {
    *listHead = newNode;
    return;
  }

  newNode->next = *listHead;
  (*listHead)->prev = newNode;
  *listHead = newNode;
}

void insertEnd(Node** listHead, int value) {
  Node* newNode = createNode(value);

  if (*listHead == NULL) {
    *listHead = newNode;
    return;
  }

  Node* tmpNode = *listHead;
  while (tmpNode->next != NULL) {
    tmpNode = tmpNode->next;
  }
  tmpNode->next = newNode;
  newNode->prev = tmpNode;
}

void removeStart(Node** listHead) {
  if (*listHead == NULL) {
    return;
  }

  Node* toRemove = *listHead;
  *listHead = (*listHead)->next; 
  if (*listHead != NULL) {
    (*listHead)->prev = NULL;   }

  free(toRemove);
}

void removeEnd(Node** listHead) {
  if (*listHead == NULL) {
    return;
  }

  Node* tmpNode = *listHead;
  
   if (tmpNode->next == NULL) {
    *listHead = NULL;
    free(tmpNode);
    return;
  }

  while (tmpNode->next != NULL) {
    tmpNode = tmpNode->next;
  }

  tmpNode->prev->next = NULL;
  free(tmpNode);
}

void freeList(Node** listHead) {
  Node* current = *listHead;
  Node* nextNode;

  while (current != NULL) {
    nextNode = current->next;
    free(current);
    current = nextNode;
  }

  *listHead = NULL; }

void printList(Node* listHead) {
  Node* tmp = listHead;
  printf("Lista: ");
  while (tmp != NULL) {
    printf("%d <-> ", tmp->val);
    tmp = tmp->next;
  }
  printf("NULL\n");
}

int main() {
  Node* minhaLista = NULL;

  insertStart(&minhaLista, 10);
  insertStart(&minhaLista, 20);
  insertEnd(&minhaLista, 30);
  printList(minhaLista); 
  
  removeStart(&minhaLista);
  printList(minhaLista); 

  removeEnd(&minhaLista);
  printList(minhaLista); 

  freeList(&minhaLista);
  printList(minhaLista); 

  return 0;
}
