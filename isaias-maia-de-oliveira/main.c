#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

No *criarNo(int valor) {
    No *novo = (No *) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo->valor = valor;
    novo->anterior = NULL;
    novo->proximo = NULL;

    return novo;
}

void inserirFinal(No **inicio, int valor) {
    No *novo = criarNo(valor);

    if (*inicio == NULL) {
        *inicio = novo;
        return;
    }

    No *atual = *inicio;

    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }

    atual->proximo = novo;
    novo->anterior = atual;
}

void imprimirLista(No *inicio) {
    No *atual = inicio;

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }

    printf("\n");
}

void liberarLista(No *inicio) {
    No *atual = inicio;

    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

int main() {
    No *inicio = NULL;

    inserirFinal(&inicio, 10);
    inserirFinal(&inicio, 20);
    inserirFinal(&inicio, 30);

    printf("Lista: ");
    imprimirLista(inicio);

    liberarLista(inicio);

    return 0;
}
