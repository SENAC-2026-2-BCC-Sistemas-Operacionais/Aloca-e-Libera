#include <stdio.h>
#include <stdlib.h>

typedef struct No {
     int valor;
     struct No *ant;
     struct No *prox;
} No ;

typedef struct {
    No *inicio;
    No *fim;
} Lista ;

void inicializarLista(Lista *lista) {
  lista->inicio = NULL;
  lista->fim = NULL;
}

void inserirFim(Lista *lista, int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    novo->valor = valor;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim != NULL) {
        lista->fim->prox = novo;
    } else {
        lista->inicio = novo;
    }

    lista->fim = novo;
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
        	free(atual);
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

