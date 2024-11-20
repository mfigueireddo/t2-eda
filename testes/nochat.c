#include <stdio.h>
#include <stdlib.h>

#define MIN_DEGREE 2 

struct No {
    int* chaves;
    int grauMin;
    struct No** filhos;
    int qtdChaves;
    int folha;
    struct No* prox;
};

struct Arvore {
    struct No* root;
    int grauMin;
};

typedef struct No No;
typedef struct Arvore Arvore;

No* criaNo(int grauMin, int folha);
Arvore* criaArvore(int grauMin);
void imprimeArvore(No* node);
void splitChild(No* parent, int i, No* child);
void insertNonFull(No* No, int key);
void insereNo(Arvore* arvore, int key);

int main(void){

    Arvore* tree = criaArvore(MIN_DEGREE);

    insereNo(tree, 10);
    insereNo(tree, 20);
    insereNo(tree, 5);
    insereNo(tree, 6);
    insereNo(tree, 30);
    insereNo(tree, 15);
    insereNo(tree, 25);
    insereNo(tree, 35);
    insereNo(tree, 40);
    insereNo(tree, 50);
    imprimeArvore(tree->root);

    return 0;
}

// Função para criar um novo nó
No* criaNo(int grauMin, int folha) {
    No* newNo = (No*)malloc(sizeof(No));
    newNo->grauMin = grauMin;
    newNo->folha = folha;
    newNo->chaves = (int*)malloc((2 * grauMin - 1) * sizeof(int));  // 2t-1 chaves no nó
    newNo->filhos = (No**)malloc((2 * grauMin) * sizeof(No*));  // 2t filhos no nó
    newNo->qtdChaves = 0;
    newNo->prox = NULL;
    return newNo;
}

// Função para criar a árvore B+ (inicializa com grauMin=2)
Arvore* criaArvore(int grauMin) {
    Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));
    arvore->grauMin = grauMin;
    arvore->root = criaNo(grauMin, 1);  // A raiz é uma folha inicialmente
    return arvore;
}

// Função para exibir a árvore B+ e suas chaves
void imprimeArvore(No* node) {
    if (node == NULL) return;

    // Imprime o ponteiro para o nó
    printf("Ponteiro para o nó: %p\n", node);

    // Imprime as chaves do nó
    printf("Conteúdo do nó - ");
    for (int i = 0; i < node->qtdChaves; i++) {
        printf("%d ", node->chaves[i]);
    }

    // Imprime os ponteiros para os filhos (se o nó não for folha)
    if (!node->folha) {
        printf("\nPonteiros para os filhos - ");
        for (int i = 0; i <= node->qtdChaves; i++) {
            printf("%p ", node->filhos[i]);
        }
    }

    printf("\n\n");

    // Se o nó for interno (não folha), percorre os filhos
    if (!node->folha) {
        for (int i = 0; i <= node->qtdChaves; i++) {
            imprimeArvore(node->filhos[i]);
        }
    }
}

// Função para dividir o filho de um nó durante a inserção
void splitChild(No* parent, int i, No* child) {
    int grauMin = child->grauMin;
    No* newChild = criaNo(grauMin, child->folha);
    newChild->qtdChaves = grauMin - 1;

    // Copiar as chaves do filho para o novo filho
    for (int j = 0; j < grauMin - 1; j++) {
        newChild->chaves[j] = child->chaves[j + grauMin];
    }

    // Copiar os filhos do filho para o novo filho, se não for folha
    if (!child->folha) {
        for (int j = 0; j < grauMin; j++) {
            newChild->filhos[j] = child->filhos[j + grauMin];
        }
    }

    child->qtdChaves = grauMin - 1;

    // Deslocar filhos no nó pai
    for (int j = parent->qtdChaves; j >= i + 1; j--) {
        parent->filhos[j + 1] = parent->filhos[j];
    }
    parent->filhos[i + 1] = newChild;

    // Deslocar chaves no nó pai
    for (int j = parent->qtdChaves - 1; j >= i; j--) {
        parent->chaves[j + 1] = parent->chaves[j];
    }

    // Promover a chave central para o nó pai
    parent->chaves[i] = child->chaves[grauMin - 1];
    parent->qtdChaves += 1;
}

// Função para inserir uma chave em um nó não cheio
void insertNonFull(No* No, int key) {
    int i = No->qtdChaves - 1;

    // Se o nó for folha, insere a chave
    if (No->folha) {
        while (i >= 0 && No->chaves[i] > key) {
            No->chaves[i + 1] = No->chaves[i];
            i--;
        }
        No->chaves[i + 1] = key;
        No->qtdChaves += 1;
    } 
    else { // Se não for folha, desce na árvore
        while (i >= 0 && No->chaves[i] > key) {
            i--;
        }
        i++;
        // Verifica se o filho está cheio e precisa ser dividido
        if (No->filhos[i]->qtdChaves == 2 * No->grauMin - 1) {
            splitChild(No, i, No->filhos[i]);
            if (No->chaves[i] < key) {
                i++;
            }
        }
        insertNonFull(No->filhos[i], key);
    }
}

// Função para inserir uma chave na árvore B+
void insereNo(Arvore* arvore, int key) {
    No* root = arvore->root;
    
    // Se a raiz está cheia, divide-a
    if (root->qtdChaves == 2 * arvore->grauMin - 1) {
        No* newRoot = criaNo(arvore->grauMin, 0);  // Cria um novo nó não folha
        newRoot->filhos[0] = root;
        splitChild(newRoot, 0, root);
        insertNonFull(newRoot, key);
        arvore->root = newRoot;
    } 
    else {
        insertNonFull(root, key);
    }
}