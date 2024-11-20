#include <stdio.h>
#include <stdlib.h>

struct Arvore {
   int *chaves;
   struct Arvore **filhos;
   int eh_folha;
   int qtd_chaves;
};

typedef struct Arvore Arvore;

Arvore* criaNo(void);
void imprimeArvore(Arvore *no);
void ordena(int *chaves, int qtd_chaves);
int cisaoFilho(Arvore *raiz, Arvore *atual, int i);
void insereNo(Arvore* raiz, int chave);

int main(void) {
   
   Arvore* raiz = NULL;

   insereNo(raiz, 10);
   insereNo(raiz, 20);
   insereNo(raiz, 30);
   insereNo(raiz, 40);
   insereNo(raiz, 50);

   imprimeArvore(raiz);

   return 0;
}

Arvore* criaNo(void){
    Arvore* novo_no = (Arvore*)malloc(sizeof(Arvore));
    if (novo_no == NULL) exit(1);

    novo_no->chaves = (int*)malloc(2 * sizeof(int));
    for (int i = 0; i < 2; i++) {
      novo_no->chaves[i] = -1;
    }

    novo_no->filhos = (Arvore**)malloc(3 * sizeof(Arvore*));
    for (int i = 0; i < 3; i++) {
      novo_no->filhos[i] = NULL;
    }

    novo_no->eh_folha = 1;
    novo_no->qtd_chaves = 0;

    return novo_no;
}

void imprimeArvore(Arvore *no){
    if (no == NULL) return;

    int i;

    // Imprime o ponteiro para o nó atual
    printf("Nó (%p): ", no);

    // Imprime as chaves do nó
    printf("\nChaves - ");
    for (i = 0; i < no->qtd_chaves; i++) {
        printf("%d ", no->chaves[i]);
    }

    // Imprime os ponteiros para os filhos do nó
    printf("\nPonteiros - ");
    for (i = 0; i <= no->qtd_chaves; i++) {
        if (no->eh_folha == 0) {
            printf("%p ", (void *)no->filhos[i]);
        }
    }
    printf("\n");

    // Chamada recursiva para os filhos, se o nó não for uma folha
    if (no->eh_folha == 0) {
        for (i = 0; i <= no->qtd_chaves; i++) {
            imprimeArvore(no->filhos[i]);
        }
    }
}

void ordena(int *chaves, int qtd_chaves){
   int i, j, aux;
   for (i = 0; i < qtd_chaves; i++) {
      for (j = i; j <= qtd_chaves; j++) {
         if (chaves[i] > chaves[j]) {
            aux = chaves[i];
            chaves[i] = chaves[j];
            chaves[j] = aux;
         }
      }
   }
}

int cisaoFilho(Arvore *raiz, Arvore *atual, int i){

    int meio;
    Arvore *nova_raiz, *filho_split;

    // Novo nó
    Arvore* novo_no = criaNo();
    novo_no->eh_folha = 1;

    // Split na raiz
    if (i == -1) {

        // Copia a chave a ser deslocada
        meio = atual->chaves[1];
        atual->chaves[1] = -1;
        atual->qtd_chaves--;

        // Cria nova raiz
        Arvore* nova_raiz = criaNo();
        nova_raiz->eh_folha = 0;

        // Raiz passa a ser folha
        atual->eh_folha = 1;

        // Copia a parte da direita do nó para um novo nó
        novo_no->chaves[0] = atual->chaves[2];
        novo_no->filhos[0] = atual->filhos[2];
        novo_no->qtd_chaves++;
        atual->chaves[2] = -1;
        atual->filhos[2] = NULL;
        atual->qtd_chaves--;

        // Ajusta a nova raiz
        nova_raiz->chaves[0] = meio;
        nova_raiz->qtd_chaves++;
        nova_raiz->filhos[0] = atual;
        nova_raiz->filhos[1] = novo_no;

        raiz = nova_raiz;
    }
    else { 
        // i indica o índice do filho a ser splitado
        filho_split = atual->filhos[i];
        meio = filho_split->chaves[1];
        filho_split->chaves[1] = -1;
        filho_split->qtd_chaves--;

        // Copia a parte da direita do nó para um novo nó
        novo_no->chaves[0] = filho_split->chaves[2];
        novo_no->filhos[0] = filho_split->filhos[2];
        novo_no->qtd_chaves++;
        filho_split->chaves[2] = -1;
        filho_split->filhos[2] = NULL;
        filho_split->qtd_chaves--;

        // Ajusta o pai
        atual->filhos[i] = filho_split;
        atual->filhos[i+1] = novo_no;
    }

   return meio;
}

void insereNo(Arvore* raiz, int chave){

   int i, meio;
   Arvore* atual = raiz;
   // Se a árvore estiver vazia
   if (raiz == NULL) {
      raiz = criaNo();
   } 
   // Se a árvore não estiver vazia
   else{
      // Se precisar de split
      if (atual->eh_folha == 1 && atual->qtd_chaves == 2) {
         meio = cisaoFilho(raiz, atual, -1);
         atual = raiz;
         // Procura a posição para encaixar a chave
         for (i=0; i < atual->qtd_chaves; i++) {
            if (chave > atual->chaves[i] && chave < atual->chaves[i + 1]){ i++; break; }
            else if (chave < atual->chaves[0]) break;
            else continue;
         } 
         atual = atual->filhos[i];
      }
      // Se não precisar de split
      else {
         // Enquanto não for folha
         while (atual->eh_folha == 0) {
            // Procura a posição para encaixar a chave
            for (i = 0; i < atual->qtd_chaves; i++) {
               if (chave > atual->chaves[i] && chave < atual->chaves[i + 1]) { i++; break; }
               else if (chave < atual->chaves[0]) break;
               else continue;
            }
            // Se precisar de split
            if (atual->filhos[i]->qtd_chaves == 2) {
               meio = cisaoFilho(raiz, atual, i);
               atual->chaves[atual->qtd_chaves] = meio;
               atual->qtd_chaves++;
               continue;
            } 
            // Se não precisar de split
            else atual = atual->filhos[i];
         }
      }
   }

   atual->chaves[atual->qtd_chaves] = chave;
   ordena(atual->chaves, atual->qtd_chaves);
   atual->qtd_chaves++;
}