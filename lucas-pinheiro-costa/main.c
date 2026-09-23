#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int dado;
    struct Node* proximo;
} Node;

Node* criarNo(int valor) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    novoNo->dado = valor;
    novoNo->proximo = NULL;
    return novoNo;
}

void inserirInicio(Node** cabeca, int valor) {
    Node* novoNo = criarNo(valor);
    novoNo->proximo = *cabeca;
    *cabeca = novoNo;
    printf("Elemento %d inserido no inicio.\n", valor);
}

void inserirFim(Node** cabeca, int valor) {
    Node* novoNo = criarNo(valor);

    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        Node* atual = *cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
    printf("Elemento %d inserido no fim.\n", valor);
}

void imprimirLista(Node* cabeca) {
    Node* atual = cabeca;
    printf("\nLista atual: ");
    while (atual != NULL) {
        printf("%d -> ", atual->dado);
        atual = atual->proximo;
    }
    printf("NULL\n\n");
}

int main() {
    Node* cabeca = NULL;
    int opcao, valor;

    do {
        printf("=== MENU LISTA LIGADA ===\n");
        printf("1. Inserir no inicio\n");
        printf("2. Inserir no fim\n");
        printf("3. Imprimir lista\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserirInicio(&cabeca, valor);
                break;
            
            case 2:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserirFim(&cabeca, valor);
                break;
            
            case 3:
                imprimirLista(cabeca);
                break;
            
            case 4:
                printf("Encerrando o programa e liberando memoria...\n");
                break;
            
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        printf("\n");
    } while (opcao != 4);

    Node* atual = cabeca;
    Node* proximoNo;
    while (atual != NULL) {
        proximoNo = atual->proximo;
        free(atual);
        atual = proximoNo;
    }

    return 0;
}
