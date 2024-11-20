#include <stdio.h>
#include <stdlib.h>

#define ORDEM ?

struct Nodo{

};

struct Arvore{

};

typedef struct Nodo Nodo;
typedef struct Arvore Arvore;

int main(void){

    return 0;
}

// Inserção

void insereNo(Arvore* raiz, int chave){
    Nodo* atual = raiz->ptr;
    int i;
    // Se a árvore estiver vazia
    if (atual == NULL){
        raiz->ptr = criaNo();
        return;
    }

    for(i=0; i < atual->qtd_chaves; i++){
        if (atual->chaves)
    }

    
    // Localizar a folha dentro da qual a chave deve ser inserida
    // Localizar a posição de inserção dentro da folha
    // Inserir a chave
    // Se, após a inserção, a folha estiver completa, realizar a cisão
}