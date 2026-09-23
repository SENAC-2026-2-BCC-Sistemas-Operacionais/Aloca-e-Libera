#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int dado;
    struct No* anterior;
    struct No* proximo;
} No;

No* criar_no(int valor) {
    No* novo = (No*) malloc(sizeof(No));
    if (novo != NULL) {
        novo->dado = valor;
        novo->anterior = NULL;
        novo->proximo = NULL;
    }
    return novo;
}

void inserir_inicio(No** inicio, int valor) {
    No* novo = criar_no(valor);
    if (novo == NULL) return;
    
    if (*inicio != NULL) {
        (*inicio)->anterior = novo;
        novo->proximo = *inicio;
    }
    *inicio = novo;
}

void liberar_lista(No** inicio) {
    No* atual = *inicio;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    *inicio = NULL;
}


int main() {
    No* minha_lista = NULL;
    inserir_inicio(&minha_lista, 10);
    liberar_lista(&minha_lista);

    return 0;
}