    #include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

void inicializarLista(Lista *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

void inserirInicio(Lista *lista, int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    novo->valor = valor;
    novo->anterior = NULL;
    novo->proximo = lista->inicio;

    if (lista->inicio != NULL) {
        lista->inicio->anterior = novo;
    } else {
        lista->fim = novo;
    }

    lista->inicio = novo;
    lista->tamanho++;

    printf("Elemento %d inserido no inicio.\n", valor);
}

void inserirFim(Lista *lista, int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    novo->valor = valor;
    novo->proximo = NULL;
    novo->anterior = lista->fim;

    if (lista->fim != NULL) {
        lista->fim->proximo = novo;
    } else {
        lista->inicio = novo;
    }

    lista->fim = novo;
    lista->tamanho++;

    printf("Elemento %d inserido no fim.\n", valor);
}

No *buscar(Lista *lista, int valor) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        if (atual->valor == valor) {
            return atual;
        }

        atual = atual->proximo;
    }

    return NULL;
}

void remover(Lista *lista, int valor) {
    No *atual = buscar(lista, valor);

    if (atual == NULL) {
        printf("Elemento %d nao encontrado.\n", valor);
        return;
    }

    if (atual->anterior != NULL) {
        atual->anterior->proximo = atual->proximo;
    } else {
        lista->inicio = atual->proximo;
    }

    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    } else {
        lista->fim = atual->anterior;
    }

    free(atual);
    lista->tamanho--;

    printf("Elemento %d removido.\n", valor);
}

void imprimirInicioFim(Lista *lista) {
    No *atual = lista->inicio;

    printf("\nLista do inicio para o fim:\n");

    if (atual == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    while (atual != NULL) {
        printf("%d", atual->valor);

        if (atual->proximo != NULL) {
            printf(" <-> ");
        }

        atual = atual->proximo;
    }

    printf("\n");
}

void imprimirFimInicio(Lista *lista) {
    No *atual = lista->fim;

    printf("\nLista do fim para o inicio:\n");

    if (atual == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    while (atual != NULL) {
        printf("%d", atual->valor);

        if (atual->anterior != NULL) {
            printf(" <-> ");
        }

        atual = atual->anterior;
    }

    printf("\n");
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int main() {
    Lista lista;
    int opcao;
    int valor;

    inicializarLista(&lista);

    do {
        printf("\n========== LISTA DUPLAMENTE ENCADEADA ==========\n");
        printf("1. Inserir no inicio\n");
        printf("2. Inserir no fim\n");
        printf("3. Remover elemento\n");
        printf("4. Buscar elemento\n");
        printf("5. Imprimir inicio -> fim\n");
        printf("6. Imprimir fim -> inicio\n");
        printf("7. Mostrar tamanho\n");
        printf("0. Sair\n");
        printf("===============================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserirInicio(&lista, valor);
                break;

            case 2:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserirFim(&lista, valor);
                break;

            case 3:
                printf("Digite o valor a remover: ");
                scanf("%d", &valor);
                remover(&lista, valor);
                break;

            case 4:
                printf("Digite o valor a buscar: ");
                scanf("%d", &valor);

                if (buscar(&lista, valor) != NULL) {
                    printf("Elemento %d encontrado.\n", valor);
                } else {
                    printf("Elemento %d nao encontrado.\n", valor);
                }

                break;

            case 5:
                imprimirInicioFim(&lista);
                break;

            case 6:
                imprimirFimInicio(&lista);
                break;

            case 7:
                printf("Tamanho da lista: %d\n", lista.tamanho);
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    liberarLista(&lista);

    return 0;
}

