#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define TAMANHO_BUFFER (16 * 1024)

static unsigned char buffer[TAMANHO_BUFFER];

typedef struct Bloco {
    size_t tamanho;
    int livre;
    struct Bloco *proximo;
} Bloco;

static Bloco *primeiro = NULL;

void *aloca(size_t tamanho) {
    Bloco *atual = primeiro;

    if (tamanho == 0)
        return NULL;

    while (atual != NULL) {
        if (atual->livre && atual->tamanho >= tamanho) {
            atual->livre = 0;
            return (void *)(atual + 1);
        }

        atual = atual->proximo;
    }

    size_t ocupado = 0;
    atual = primeiro;

    while (atual != NULL) {
        ocupado += sizeof(Bloco) + atual->tamanho;
        atual = atual->proximo;
    }

    if (ocupado + sizeof(Bloco) + tamanho > TAMANHO_BUFFER)
        return NULL;

    Bloco *novo = (Bloco *)(buffer + ocupado);

    novo->tamanho = tamanho;
    novo->livre = 0;
    novo->proximo = NULL;

    if (primeiro == NULL) {
        primeiro = novo;
    } else {
        atual = primeiro;

        while (atual->proximo != NULL)
            atual = atual->proximo;

        atual->proximo = novo;
    }

    return (void *)(novo + 1);
}

void libera(void *memoria) {
    if (memoria == NULL)
        return;

    Bloco *bloco = (Bloco *)memoria - 1;
    bloco->livre = 1;
}


typedef struct No {
    int valor;
    struct No *ant;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
} Lista;

void inicializarLista(Lista *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
}

void inserirFim(Lista *lista, int valor) {
    No *novo = aloca(sizeof(No));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    novo->valor = valor;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim == NULL) {
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        lista->fim->prox = novo;
        lista->fim = novo;
    }
}

void imprimirLista(Lista *lista) {
    No *atual = lista->inicio;

    printf("Lista: ");

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->prox;
    }

    printf("\n");
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        No *proximo = atual->prox;
        libera(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
}

int main() {
    Lista lista;

    inicializarLista(&lista);

    inserirFim(&lista, 10);
    inserirFim(&lista, 20);
    inserirFim(&lista, 30);

   imprimirLista(&lista);

   liberarLista(&lista);

    return 0;
}
