#include <stdio.h>
#include <stddef.h>

#define TAM_BUFFER 16384
#define ALINHA(x) (((x) + 15) & ~(size_t)15)   

typedef struct Bloco {
    size_t tamanho;  
    int livre;     
} Bloco;

#define CAB ALINHA(sizeof(Bloco)) 

static _Alignas(16) unsigned char buffer[TAM_BUFFER];
static int iniciado = 0;

static Bloco *proximo(Bloco *b) {
    return (Bloco *)((unsigned char *)b + CAB + b->tamanho);
}

void *aloca(size_t tam) {
    if (tam == 0 || tam > TAM_BUFFER) return NULL;
    tam = ALINHA(tam);

    if (!iniciado) {
        Bloco *p = (Bloco *)buffer;
        p->tamanho = TAM_BUFFER - CAB;
        p->livre = 1;
        iniciado = 1;
    }

    Bloco *b = (Bloco *)buffer;
    while ((unsigned char *)b < buffer + TAM_BUFFER) {
        if (b->livre && b->tamanho >= tam) {
            if (b->tamanho >= tam + CAB + 16) {
                Bloco *resto = (Bloco *)((unsigned char *)b + CAB + tam);
                resto->tamanho = b->tamanho - tam - CAB;
                resto->livre = 1;
                b->tamanho = tam;
            }
            b->livre = 0;
            return (unsigned char *)b + CAB;
        }
        b = proximo(b);
    }
    return NULL; 
}

void libera(void *ptr) {
    if (ptr == NULL) return;

    Bloco *b = (Bloco *)((unsigned char *)ptr - CAB);
    b->livre = 1;

    b = (Bloco *)buffer;
    while ((unsigned char *)b < buffer + TAM_BUFFER) {
        Bloco *prox = proximo(b);
        if ((unsigned char *)prox < buffer + TAM_BUFFER && b->livre && prox->livre)
            b->tamanho += CAB + prox->tamanho;  
        else
            b = prox;
    }
}

//exemplo

typedef struct No {
    int dado;
    struct No *prev, *next;
} No;

No *insere_inicio(No *head, int valor) {
    No *n = aloca(sizeof(No));
    if (n == NULL) return head;
    n->dado = valor;
    n->prev = NULL;
    n->next = head;
    if (head) head->prev = n;
    return n;
}

int main(void) {
    No *lista = NULL;
    lista = insere_inicio(lista, 30);
    lista = insere_inicio(lista, 20);
    lista = insere_inicio(lista, 10);

    for (No *n = lista; n; n = n->next) printf("%d ", n->dado);
    printf("\n");

    while (lista) {
        No *tmp = lista;
        lista = lista->next;
        libera(tmp);
    }
    printf("memoria liberada\n");

    void *grande = aloca(16000);
    printf("aloca(16000) %s\n", grande ? "ok" : "falhou");
    libera(grande);
    return 0;
}