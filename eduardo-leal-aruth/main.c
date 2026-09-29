#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_BUFFER 16384

double buffer_global[TAMANHO_BUFFER / sizeof(double)];

typedef struct BlocoMemoria {
	size_t tamanho;
	int livre;
	struct BlocoMemoria *next;
} BlocoMemoria;

BlocoMemoria *inicio_memoria = (void*)buffer_global;
int memoria_iniciada = 0;

void* aloca(size_t tamanho) {
	if(!memoria_iniciada) {
		inicio_memoria->tamanho = TAMANHO_BUFFER - sizeof(BlocoMemoria);
		inicio_memoria->livre = 1;
		inicio_memoria->next = NULL;
		memoria_iniciada = 1;
	}

	BlocoMemoria *atual = inicio_memoria;
	while (atual != NULL) {
		if (atual->livre && atual->tamanho >= tamanho) {
			if (atual->tamanho > tamanho + sizeof(BlocoMemoria)) {
				BlocoMemoria *novo_bloco = (BlocoMemoria*)((char*)(atual + 1) + tamanho);
				novo_bloco->tamanho = atual->tamanho - tamanho - sizeof(BlocoMemoria);
				novo_bloco->livre = 1;
				novo_bloco->next = atual->next;

				atual->tamanho = tamanho;
				atual->next = novo_bloco;
			}

			atual->livre = 0;
			return (void*)(atual + 1);
		}
		atual = atual->next;
	}
	return NULL;
}

void libera (void *ptr) {
	if (ptr = NULL) return;
	BlocoMemoria *bloco = (BlocoMemoria*)ptr - 1;
	bloco->livre = 1;
}


typedef struct Node {
	int dado;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct {
	Node *head;
	Node *tail;
} Lista;

void iniciar_lista(Lista *l) {
	l->head = NULL;
	l->tail = NULL;
}

void adicionar_node(Lista *l, int valor) {
	Node *novo_node = (Node*) aloca(sizeof(Node));
	
	if (novo_node == NULL) {
		printf("alocação falhou\n");
		return;
	}
	
	novo_node->dado = valor;
	novo_node->next = NULL;
	novo_node->prev = l->tail;

	if (l->tail != NULL) {
		l->tail->next = novo_node;
	} else {
		l->head = novo_node;
	}
	l->tail = novo_node;
}

void imprimir_lista (Lista *l) {
	Node *atual = l->head;
	printf("Elementos da lista: ");
	while (atual != NULL) {
		printf("%d ", atual->dado);
		atual = atual->next;
	}
	printf("\n");
}

void liberar_lista (Lista *l) {
	Node *atual = l->head;
	while (atual != NULL) {
		Node *proximo_temp = atual->next;
		libera(atual);
		atual = proximo_temp;
	}
	l->head = NULL;
	l->tail = NULL;
}

int main() {
	Lista minha_lista;
	iniciar_lista(&minha_lista);

	adicionar_node(&minha_lista, 10);
	adicionar_node(&minha_lista, 20);
	adicionar_node(&minha_lista, 40);

	imprimir_lista(&minha_lista);

	liberar_lista(&minha_lista);

	return 0;
}
