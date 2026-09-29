#include <stdio.h>
#include <stddef.h>

#define TAMANHO_BUFFER 16384

static unsigned char buffer[TAMANHO_BUFFER];

typedef struct Bloco {
    size_t tamanho;
    int livre;
    struct Bloco *proximo;
} Bloco;

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
} Lista;

static Bloco *primeiro = (Bloco *)buffer;

void inicia_memoria(void)
{
    primeiro->tamanho = TAMANHO_BUFFER - sizeof(Bloco);
    primeiro->livre = 1;
    primeiro->proximo = NULL;
}

void *aloca(size_t tamanho)
{
    Bloco *atual = primeiro;

    while (atual != NULL) {
        if (atual->livre && atual->tamanho >= tamanho) {
            if (atual->tamanho >= tamanho + sizeof(Bloco) + 1) {
                Bloco *novo;

                novo = (Bloco *)((char *)atual +
                    sizeof(Bloco) + tamanho);

                novo->tamanho = atual->tamanho -
                    tamanho - sizeof(Bloco);
                novo->livre = 1;
                novo->proximo = atual->proximo;

                atual->tamanho = tamanho;
                atual->proximo = novo;
            }

            atual->livre = 0;
            return (char *)atual + sizeof(Bloco);
        }

        atual = atual->proximo;
    }

    return NULL;
}

void libera(void *ptr)
{
    Bloco *atual = primeiro;

    if (ptr == NULL)
        return;

    while (atual != NULL) {
        if ((char *)atual + sizeof(Bloco) == ptr) {
            atual->livre = 1;
            return;
        }

        atual = atual->proximo;
    }
}

void inicia_lista(Lista *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
}

void insere(Lista *lista, int valor)
{
    No *novo = aloca(sizeof(No));

    if (novo == NULL)
        return;

    novo->valor = valor;
    novo->proximo = NULL;
    novo->anterior = lista->fim;

    if (lista->fim != NULL)
        lista->fim->proximo = novo;
    else
        lista->inicio = novo;

    lista->fim = novo;
}

void remove_no(Lista *lista, int valor)
{
    No *atual = lista->inicio;

    while (atual != NULL) {
        if (atual->valor == valor) {
            if (atual->anterior != NULL)
                atual->anterior->proximo = atual->proximo;
            else
                lista->inicio = atual->proximo;

            if (atual->proximo != NULL)
                atual->proximo->anterior = atual->anterior;
            else
                lista->fim = atual->anterior;

            libera(atual);
            return;
        }

        atual = atual->proximo;
    }
}

void libera_lista(Lista *lista)
{
    No *atual = lista->inicio;

    while (atual != NULL) {
        No *proximo = atual->proximo;
        libera(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
}

void imprime(Lista *lista)
{
    No *atual = lista->inicio;

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }

    printf("\n");
}

int main(void)
{
    Lista lista;

    inicia_memoria();
    inicia_lista(&lista);

    insere(&lista, 10);
    insere(&lista, 20);
    insere(&lista, 30);

    imprime(&lista);

    remove_no(&lista, 20);

    imprime(&lista);

    libera_lista(&lista);

    return 0;
}
