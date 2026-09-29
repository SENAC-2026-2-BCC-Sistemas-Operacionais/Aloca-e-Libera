#include <stdio.h>
#include <stddef.h>

#define TAM_MEMORIA (16 * 1024)

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

static unsigned char memoria[TAM_MEMORIA];
static Bloco *inicio_memoria = NULL;


/* arredonda o tamanho para múltiplos de 8 */
size_t alinhar(size_t tamanho)
{
    return (tamanho + 7) & ~7;
}


/* prepara os 16 KiB como um único bloco livre */
void iniciar_memoria(void)
{
    inicio_memoria = (Bloco *) memoria;

    inicio_memoria->tamanho =
        TAM_MEMORIA - sizeof(Bloco);

    inicio_memoria->livre = 1;
    inicio_memoria->proximo = NULL;
}


/* funciona como malloc */
void *aloca(size_t tamanho)
{
    Bloco *atual;

    if (tamanho == 0)
        return NULL;

    if (inicio_memoria == NULL)
        iniciar_memoria();

    tamanho = alinhar(tamanho);

    atual = inicio_memoria;

    while (atual != NULL) {

        if (atual->livre &&
            atual->tamanho >= tamanho) {

            /* separa o bloco se ainda sobrar espaço */
            if (atual->tamanho >
                tamanho + sizeof(Bloco)) {

                Bloco *novo;

                novo = (Bloco *)
                    ((unsigned char *)(atual + 1)
                    + tamanho);

                novo->tamanho =
                    atual->tamanho
                    - tamanho
                    - sizeof(Bloco);

                novo->livre = 1;
                novo->proximo = atual->proximo;

                atual->tamanho = tamanho;
                atual->proximo = novo;
            }

            atual->livre = 0;

            return atual + 1;
        }

        atual = atual->proximo;
    }

    return NULL;
}


/* funciona como free */
void libera(void *ponteiro)
{
    Bloco *bloco;
    Bloco *atual;

    if (ponteiro == NULL)
        return;

    bloco = (Bloco *) ponteiro - 1;
    bloco->livre = 1;

    /* junta blocos livres que estão lado a lado */
    atual = inicio_memoria;

    while (atual != NULL &&
           atual->proximo != NULL) {

        if (atual->livre &&
            atual->proximo->livre) {

            atual->tamanho +=
                sizeof(Bloco)
                + atual->proximo->tamanho;

            atual->proximo =
                atual->proximo->proximo;

        } else {
            atual = atual->proximo;
        }
    }
}


/* insere no final da lista */
void inserir(No **inicio, No **fim, int valor)
{
    No *novo = aloca(sizeof(No));

    if (novo == NULL) {
        printf("Sem memoria disponivel\n");
        return;
    }

    novo->valor = valor;
    novo->anterior = *fim;
    novo->proximo = NULL;

    if (*fim == NULL) {
        *inicio = novo;
    } else {
        (*fim)->proximo = novo;
    }

    *fim = novo;
}


/* remove um valor da lista */
void remover(No **inicio, No **fim, int valor)
{
    No *atual = *inicio;

    while (atual != NULL) {

        if (atual->valor == valor) {

            if (atual->anterior != NULL)
                atual->anterior->proximo =
                    atual->proximo;
            else
                *inicio = atual->proximo;

            if (atual->proximo != NULL)
                atual->proximo->anterior =
                    atual->anterior;
            else
                *fim = atual->anterior;

            libera(atual);

            return;
        }

        atual = atual->proximo;
    }
}


void mostrar(No *inicio)
{
    No *atual = inicio;

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }

    printf("\n");
}


void mostrar_reverso(No *fim)
{
    No *atual = fim;

    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->anterior;
    }

    printf("\n");
}


int main(void)
{
    No *inicio = NULL;
    No *fim = NULL;

    inserir(&inicio, &fim, 10);
    inserir(&inicio, &fim, 20);
    inserir(&inicio, &fim, 30);

    printf("Lista: ");
    mostrar(inicio);

    printf("Lista ao contrario: ");
    mostrar_reverso(fim);

    remover(&inicio, &fim, 20);

    printf("Depois de remover 20: ");
    mostrar(inicio);

    return 0;
}
