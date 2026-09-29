#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#define TAM_BUFFER (16 * 1024)
#define ALINHAMENTO 16

typedef struct Bloco {
    size_t tamanho;   /* tamanho útil do bloco (sem o cabeçalho) */
    int    livre;     /* 1 = livre, 0 = ocupado */
} Bloco;

#define ALINHA(x) (((x) + (ALINHAMENTO - 1)) & ~((size_t)ALINHAMENTO - 1))
#define TAM_CABECALHO ALINHA(sizeof(Bloco))

/* Buffer global de 16KB */
static _Alignas(ALINHAMENTO) unsigned char buffer_global[TAM_BUFFER];
static int inicializado = 0;

static unsigned char* fim_buffer(void) {
    return buffer_global + TAM_BUFFER;
}

static Bloco* proximo_bloco(Bloco* b) {
    return (Bloco*)((unsigned char*)b + TAM_CABECALHO + b->tamanho);
}

static void inicializa_buffer(void) {
    Bloco* primeiro = (Bloco*)buffer_global;
    primeiro->tamanho = TAM_BUFFER - TAM_CABECALHO;
    primeiro->livre = 1;
    inicializado = 1;
}

/* Une blocos livres adjacentes para reduzir a fragmentação */
static void une_blocos_livres(void) {
    Bloco* b = (Bloco*)buffer_global;
    while ((unsigned char*)b < fim_buffer()) {
        Bloco* prox = proximo_bloco(b);
        if ((unsigned char*)prox < fim_buffer() && b->livre && prox->livre) {
            b->tamanho += TAM_CABECALHO + prox->tamanho;
            /* não avança: tenta unir com o novo vizinho também */
        } else {
            b = prox;
        }
    }
}


void* aloca(size_t tamanho) {
    if (tamanho == 0) return NULL;
    if (!inicializado) inicializa_buffer();

    tamanho = ALINHA(tamanho);

    /*  usa o primeiro bloco livre que couber */
    Bloco* b = (Bloco*)buffer_global;
    while ((unsigned char*)b < fim_buffer()) {
        if (b->livre && b->tamanho >= tamanho) {
            /* Divide o bloco se sobrar espaço para outro bloco útil */
            if (b->tamanho >= tamanho + TAM_CABECALHO + ALINHAMENTO) {
                Bloco* resto = (Bloco*)((unsigned char*)b + TAM_CABECALHO + tamanho);
                resto->tamanho = b->tamanho - tamanho - TAM_CABECALHO;
                resto->livre = 1;
                b->tamanho = tamanho;
            }
            b->livre = 0;
            return (unsigned char*)b + TAM_CABECALHO;
        }
        b = proximo_bloco(b);
    }
    return NULL; /* sem memória suficiente */
}


void libera(void* ptr) {
    if (ptr == NULL || !inicializado) return;

    /* Valida o ponteiro: precisa ser o início de um bloco ocupado */
    Bloco* b = (Bloco*)buffer_global;
    while ((unsigned char*)b < fim_buffer()) {
        if ((unsigned char*)b + TAM_CABECALHO == (unsigned char*)ptr) {
            if (!b->livre) {
                b->livre = 1;
                une_blocos_livres();
            }
            return; /* se já estava livre (double free), ignora */
        }
        b = proximo_bloco(b);
    }
    /* ponteiro não pertence ao buffer: ignora */
}


typedef struct No {
    int dado;
    struct No* anterior;
    struct No* proximo;
} No;

No* criar_no(int valor) {
    No* novo = (No*) aloca(sizeof(No));
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
        libera(temp);
    }
    *inicio = NULL;
}

int main() {
    No* minha_lista = NULL;
    inserir_inicio(&minha_lista, 10);
    liberar_lista(&minha_lista);

    return 0;
}
