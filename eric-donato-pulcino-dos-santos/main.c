#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

void inserirFim(No **inicio, int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo->valor = valor;
    novo->proximo = NULL;
    novo->anterior = NULL;

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

void mostrarInicioFim(No *inicio) {
    No *atual = inicio;

    printf("Lista do inicio para o fim: ");

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }

    printf("\n");
}

void mostrarFimInicio(No *inicio) {
    if (inicio == NULL) {
        return;
    }

    No *atual = inicio;

    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }

    printf("Lista do fim para o inicio: ");

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->anterior;
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

int main(void) {
    No *inicio = NULL;

    inserirFim(&inicio, 10);
    inserirFim(&inicio, 20);
    inserirFim(&inicio, 30);
    inserirFim(&inicio, 40);

    mostrarInicioFim(inicio);
    mostrarFimInicio(inicio);

    liberarLista(inicio);

    return 0;
}
