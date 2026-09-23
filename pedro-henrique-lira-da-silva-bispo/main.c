#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int dado;
    struct No *anterior;
    struct No *proximo;
} No;

No* criarNo(int valor) {
    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: alocação de memória falhou!\n");
        exit(1);
    }
    novoNo->dado = valor;
    novoNo->anterior = NULL;
    novoNo->proximo = NULL;
    return novoNo;
}

void inserirInicio(No **cabeca, int valor) {
    No *novoNo = criarNo(valor);
    
    if (*cabeca != NULL) {
        (*cabeca)->anterior = novoNo;
        novoNo->proximo = *cabeca;
    }
    *cabeca = novoNo;
}

void inserirFim(No **cabeca, int valor) {
    No *novoNo = criarNo(valor);
    
    if (*cabeca == NULL) {
        *cabeca = novoNo;
        return;
    }
    
    No *atual = *cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    
    atual->proximo = novoNo;
    novoNo->anterior = atual;
}

void imprimirLista(No *cabeca) {
    No *atual = cabeca;
    printf("Lista (inicio -> fim): ");
    while (atual != NULL) {
        printf("%d <-> ", atual->dado);
        atual = atual->proximo;
    }
    printf("NULL\n");
}

void liberarLista(No **cabeca) {
    No *atual = *cabeca;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *cabeca = NULL;
}

int main() {
    No *cabeca = NULL;

    inserirInicio(&cabeca, 20);
    inserirInicio(&cabeca, 10);
    inserirFim(&cabeca, 30);
    inserirFim(&cabeca, 40);

    imprimirLista(cabeca);

    liberarLista(&cabeca);

    return 0;
}
