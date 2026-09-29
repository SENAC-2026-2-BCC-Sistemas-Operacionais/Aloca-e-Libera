#include <stdio.h>
#include <stddef.h>

#define TAMANHO_HEAP (1024 * 1024)
#define ALINHAMENTO  16
#define ALINHAR(t) (((t) + (ALINHAMENTO - 1)) & ~(size_t)(ALINHAMENTO - 1))

typedef struct Bloco {
    size_t tamanho;
    int livre;
    struct Bloco *prox;
    struct Bloco *ant;
} Bloco;

#define TAM_CABECALHO ALINHAR(sizeof(Bloco))

static _Alignas(ALINHAMENTO) unsigned char heap[TAMANHO_HEAP];
static Bloco *inicio = NULL;

static void iniciar_heap(void) {
    inicio = (Bloco *)heap;
    inicio->tamanho = TAMANHO_HEAP - TAM_CABECALHO;
    inicio->livre = 1;
    inicio->prox = NULL;
    inicio->ant = NULL;
}

static void dividir_bloco(Bloco *b, size_t tam) {
    if (b->tamanho >= tam + TAM_CABECALHO + ALINHAMENTO) {
        Bloco *novo = (Bloco *)((unsigned char *)b + TAM_CABECALHO + tam);
        novo->tamanho = b->tamanho - tam - TAM_CABECALHO;
        novo->livre = 1;
        novo->prox = b->prox;
        novo->ant = b;
        if (b->prox) b->prox->ant = novo;
        b->prox = novo;
        b->tamanho = tam;
    }
}

static void fundir_com_proximo(Bloco *b) {
    Bloco *p = b->prox;
    b->tamanho += TAM_CABECALHO + p->tamanho;
    b->prox = p->prox;
    if (p->prox) p->prox->ant = b;
}

void *aloca(size_t tam) {
    if (tam == 0 || tam > TAMANHO_HEAP) return NULL;
    if (inicio == NULL) iniciar_heap();

    tam = ALINHAR(tam);

    for (Bloco *b = inicio; b != NULL; b = b->prox) {
        if (b->livre && b->tamanho >= tam) {
            dividir_bloco(b, tam);
            b->livre = 0;
            return (unsigned char *)b + TAM_CABECALHO;
        }
    }
    return NULL;
}

void libera(void *ptr) {
    if (ptr == NULL) return;

    Bloco *b = inicio;
    while (b != NULL && (unsigned char *)b + TAM_CABECALHO != (unsigned char *)ptr)
        b = b->prox;

    if (b == NULL) {
        printf("[erro] libera: ponteiro %p invalido\n", ptr);
        return;
    }
    if (b->livre) {
        printf("[erro] libera: double free em %p\n", ptr);
        return;
    }

    b->livre = 1;

    if (b->prox && b->prox->livre) fundir_com_proximo(b);
    if (b->ant && b->ant->livre) fundir_com_proximo(b->ant);
}

void mostrar_heap(void) {
    printf("---- estado da heap ----\n");
    int i = 0;
    for (Bloco *b = inicio; b != NULL; b = b->prox, i++) {
        printf("bloco %d | endereco %p | %7zu bytes | %s\n",
               i, (void *)((unsigned char *)b + TAM_CABECALHO),
               b->tamanho, b->livre ? "LIVRE" : "OCUPADO");
    }
    printf("------------------------\n\n");
}

int main(void) {
    // Tentanto arrumar
    int *a = aloca(10 * sizeof(int));
    char *s = aloca(32);
    double *d = aloca(5 * sizeof(double));

    for (int i = 0; i < 10; i++) a[i] = i * i;
    const char *msg = "ola, heap manual!";
    int k = 0;
    while (msg[k]) { s[k] = msg[k]; k++; }
    s[k] = '\0';
    for (int i = 0; i < 5; i++) d[i] = i * 1.5;

    printf("a[9] = %d | s = \"%s\" | d[4] = %.1f\n\n", a[9], s, d[4]);
    mostrar_heap();

    libera(s);
    printf("Depois de liberar s:\n");
    mostrar_heap();

    char *t = aloca(16);
    printf("t reaproveitou o espaco? %s\n\n", (void *)t == (void *)s ? "sim" : "nao");

    libera(a);
    libera(t);
    libera(d);
    printf("Depois de liberar tudo (blocos fundidos):\n");
    mostrar_heap();

    libera(d);
    return 0;
}
