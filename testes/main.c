#include <stdio.h>
#include <stdlib.h>

#define ORDEM 2

struct Node {
    int keys[ORDEM+1]; // Chaves do nó
    struct Node* children[ORDEM+1]; // Ponteiros para os filhos
    struct Node* parent;
    struct Node* next; // Próximo nó folha (apenas para folhas)
    int num_keys; // Número atual de chaves no nó
    int is_leaf; // Flag para indicar se é uma folha
};

struct Tree {
    struct Node* root; // Ponteiro para a raiz da árvore
};

typedef struct Node Node;
typedef struct Tree Tree;

Tree* createRoot();
Node* createNode(int key);
void insertNode(Tree* tree, int key);
void printTree(Node* node);

int main(void) {
    
    // Cria a árvore
    Tree* tree = createRoot();

    // Insere 10 valores na árvore
    insertNode(tree, 10);
    insertNode(tree, 20);
    insertNode(tree, 5);
    insertNode(tree, 6);
    insertNode(tree, 30);
    insertNode(tree, 15);
    insertNode(tree, 25);
    insertNode(tree, 35);
    printTree(tree->root);

    /*
    insertNode(tree, 40);
    insertNode(tree, 50);
    */

    return 0;
}

Tree* createRoot() {

    // Cria a árvore
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) exit(1);

    // Cria o nó da raiz
    Node* root = (Node*)malloc(sizeof(Node));
    if (root == NULL) exit(1);

    // Liga o nó raiz à árvore
    tree->root = root;

    // Inicializar os valores do nó raiz
    root->num_keys = 0;
    root->is_leaf = 1;
    root->next = NULL;

    // Inicializar as chaves como -1
    // Inicializar os ponteiros para os filhos como NULL
    for (int i = 0; i <= ORDEM+1; i++) {
        root->keys[i] = -1;
        root->children[i] = NULL;
    }

    return tree;
}

Node* createNode(int key) {

    // Aloca memória para o novo nó
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) exit(1);

    // Inicializa os valores do nó
    node->num_keys = 1;
    node->is_leaf = 1;
    node->next = NULL;

    // Insere a chave no nó e inicializa os espaços restantes
    node->keys[0] = key;
    for (int i = 1; i <= ORDEM + 1; i++) {
        node->keys[i] = -1;
    }

    // Inicializa os ponteiros para os filhos como NULL
    for (int i = 0; i <= ORDEM + 1; i++) {
        node->children[i] = NULL;
    }

    return node;
}

void insertNode(Tree* tree, int key) {
    // Verifica se a árvore está vazia
    if (tree->root == NULL) {
        tree->root = createNode(key);
        return;
    }

    // Ponteiros auxiliares para percorrer a árvore
    Node* current = tree->root;
    Node* parent = NULL;

    // Percorre a árvore até encontrar a posição correta
    while (!current->is_leaf) {
        parent = current;
        int i = 0;
        while (i < current->num_keys && key > current->keys[i]) {
            i++;
        }
        current = current->children[i];
    }

    // Inserção na folha
    int i = current->num_keys - 1;
    while (i >= 0 && current->keys[i] > key) {
        current->keys[i + 1] = current->keys[i];
        i--;
    }
    current->keys[i + 1] = key;
    current->num_keys++;

    // Verifica se é necessário realizar split
    while (current->num_keys > ORDEM) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) exit(1);

        new_node->is_leaf = current->is_leaf;
        int mid = ORDEM / 2;
        int promoted_key = current->keys[mid];

        new_node->num_keys = current->num_keys - mid - 1;
        current->num_keys = mid;

        for (int j = 0; j < new_node->num_keys; j++) {
            new_node->keys[j] = current->keys[mid + 1 + j];
            if (!current->is_leaf) {
                new_node->children[j] = current->children[mid + 1 + j];
            }
        }
        if (!current->is_leaf) {
            new_node->children[new_node->num_keys] = current->children[current->num_keys + 1];
        }
        if (current->is_leaf) {
            new_node->next = current->next;
            current->next = new_node;
        }

        // Split na raiz
        if (current == tree->root) {
            Node* new_root = (Node*)malloc(sizeof(Node));
            if (new_root == NULL) exit(1);

            // Configuração da nova raiz
            new_root->num_keys = 1;
            new_root->keys[0] = promoted_key;
            new_root->is_leaf = 0;
            new_root->children[0] = current;
            new_root->children[1] = new_node;

            // Atualiza o pai dos filhos (se necessário)
            current->parent = new_root;
            new_node->parent = new_root;

            tree->root = new_root;
        } else {
            // Split não é na raiz
            i = parent->num_keys - 1;
            while (i >= 0 && parent->keys[i] > promoted_key) {
                parent->keys[i + 1] = parent->keys[i];
                parent->children[i + 2] = parent->children[i + 1];
                i--;
            }
            parent->keys[i + 1] = promoted_key;
            parent->children[i + 2] = new_node;
            parent->num_keys++;

            // Atualiza o pai do novo nó
            new_node->parent = parent;

            // Verifica se o pai também precisa de split
            current = parent;
        }
    }
}

void printTree(Node* node) {
    if (node == NULL) return;

    // Imprime o ponteiro para o nó
    printf("Ponteiro para o nó: %p\n", node);

    // Imprime as chaves do nó
    printf("Conteúdo do nó - ");
    for (int i = 0; i < node->num_keys; i++) {
        printf("%d ", node->keys[i]);
    }

    // Imprime os ponteiros para os filhos (se o nó não for folha)
    if (!node->is_leaf) {
        printf("\nPonteiros para os filhos - ");
        for (int i = 0; i <= node->num_keys; i++) {
            printf("%p ", node->children[i]);
        }
    }

    printf("\n\n");

    // Se o nó for interno (não folha), percorre os filhos
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            printTree(node->children[i]);
        }
    }
}