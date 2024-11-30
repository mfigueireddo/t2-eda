#include <stdio.h>
#include <stdlib.h>

#define QTD_CHAVES 2
#define QTD_PTRS 3

struct No{
    int* chaves;
    int qtd_chaves;
    struct No** filhos;
    int eh_folha;
    struct No* prox;
};

struct Arvore{
    struct No* raiz;
};

typedef struct No No;
typedef struct Arvore Arvore;

Arvore* criaArvore(void);
No* criaNo(void);
void insere(Arvore* arvore, int chave);
void imprime(No* no);
No* encontraFolha(No* no, int chave);   
void ordena(No* no);
No* cisaoRaiz(No* raiz);
void cisao(No* raiz, No* no);
No* encontraPai(No* raiz, No* filho);
void analiseCisao(Arvore* arvore, No* folha);
void imprimeFolhas(No* atual);
void imprimeChaves(No* no);
void delete_key(Arvore* arvore, int chave);
No* buscaChave(Arvore* arvore, int chave);


int main(void){

    Arvore* arvore = criaArvore();
    insere(arvore, 8);
    insere(arvore, 5);
    insere(arvore, 10);
    insere(arvore, 7);
    insere(arvore, 12);
    insere(arvore, 9);
    insere(arvore, 15);
    insere(arvore, 20);
    insere(arvore, 30);
    insere(arvore, 11);
    insere(arvore, 13);
    insere(arvore, 6);
    insere(arvore, 25);
    insere(arvore, 35);
    insere(arvore, 16);
    insere(arvore, 17);
    insere(arvore, 18);
    insere(arvore, 19);
    insere(arvore, 21);
    insere(arvore, 22);
    imprime(arvore->raiz);
    imprimeFolhas(arvore->raiz);

    printf("\n\n** Testando a busca antes de remover a chave 10 **\n\n");

    No* no_encontrado = buscaChave(arvore, 10);

    if (no_encontrado) {
        printf("Chave %d encontrada no endereco %p com chaves: ", 10, no_encontrado);
        imprimeChaves(no_encontrado);
        printf("\n");
    } else {
        printf("Chave %d nao encontrada na arvore.\n", 10);
    }

    printf("\n\nRemovendo chave 10...\n\n");
    delete_key(arvore, 10);
    printf("\n\nÁrvore B+ apos remover 10:\n\n");
    imprime(arvore->raiz);
    imprimeFolhas(arvore->raiz);

    printf("\n\n** Testando a busca apos remover a chave 10 **\n\n");

    no_encontrado = buscaChave(arvore, 10);

    if (no_encontrado) {
        printf("Chave %d encontrada no endereco %p com chaves: ", 10, no_encontrado);
        imprimeChaves(no_encontrado);
        printf("\n");
    } else {
        printf("Chave %d nao encontrada na arvore.\n", 10);
    }


    return 0;
}

Arvore* criaArvore(void){
    Arvore* nova_arvore = (Arvore*)malloc(sizeof(Arvore));
    if (nova_arvore == NULL) exit(1);
    nova_arvore->raiz = NULL;
    return nova_arvore;
}

No* criaNo(void){
    No* novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) exit(1);
    novo_no->chaves = (int*)malloc(sizeof(int)*QTD_CHAVES+1);
    novo_no->qtd_chaves = 0;
    for(int i=0; i<QTD_CHAVES+1; i++){
        novo_no->chaves[i] = -1;
    }
    novo_no->filhos = (No**)malloc(sizeof(No*)*QTD_PTRS+1);
    for(int i=0; i<QTD_PTRS+1; i++){
        novo_no->filhos[i] = NULL;
    }
    novo_no->eh_folha = 1;
    novo_no->prox = NULL;

    return novo_no;
}

void insere(Arvore* arvore, int chave){
    // Se a árvore estiver vazia
    if (arvore->raiz == NULL){
        arvore->raiz = criaNo();
        arvore->raiz->chaves[0] = chave;
        arvore->raiz->qtd_chaves++;
    }
    // Se a árvore não estiver vazia
    else{
        // Achar a folha em que a chave deve ser inserida
        No* folha = encontraFolha(arvore->raiz, chave);
        // A primeira posição vaga será dada por folha->qtd_chaves
        folha->chaves[folha->qtd_chaves] = chave;
        folha->qtd_chaves++;
        // Ordenar o nó
        ordena(folha);
        // Se for necessário fazer uma cisão
        analiseCisao(arvore, folha);
    }
    printf("** Chave %d inserida ** \n", chave);
}

void imprime(No* no){
    if (no == NULL) return;
    printf("\nEndereço do no: %p\n", no);
    printf("Chaves: ");
    for(int i=0; i<no->qtd_chaves; i++){
        printf("%d ", no->chaves[i]);
    }
    printf("\n");
    if (!no->eh_folha){
        printf("Filhos: ");
        for(int i=0; i<QTD_PTRS+1 && no->filhos[i]!=NULL; i++){
            printf("%p ", no->filhos[i]);
        }
        printf("\n");
        // Recursão para imprimir os filhos
        for(int i=0; i<QTD_PTRS+1 && no->filhos[i]!=NULL; i++){
            imprime(no->filhos[i]);
        }
    }
}

No* encontraFolha(No* no, int chave){
    int i;
    // Se o nó não for folha
    if (!no->eh_folha){
        for(i=0; i<no->qtd_chaves; i++){
            // Redireciona para um nó à esquerda de alguma chave
            if(chave < no->chaves[i] && no->filhos[i]!=NULL) return encontraFolha(no->filhos[i], chave);
        }  
        // Redirecina para um nó à direita de alguma chave
        return encontraFolha(no->filhos[i], chave);
    }
    return no;
}

No* encontraFolhaAntigo(No* no, int chave){
    int i;
    while(!no->eh_folha){
        for(i=0; i<no->qtd_chaves; i++){
            if(chave < no->chaves[i] && no->filhos[i]!=NULL) no = no->filhos[i];
        }
        no = no->filhos[i];
    }
    return no;
}

void ordena(No* no){
    int aux;
    for(int i=0; i<no->qtd_chaves; i++){
        for(int j=0; j<no->qtd_chaves-i-1; j++){
            if (no->chaves[j] > no->chaves[j+1]){
                aux = no->chaves[j];
                no->chaves[j] = no->chaves[j+1];
                no->chaves[j+1] = aux;
            }
        }
    }
}

No* cisaoRaiz(No* raiz){

    // Nó que vai ficar "em cima"
    No* nova_raiz = criaNo();
    // Nó que vai ficar "na direita"
    No* novo_no = criaNo();

    // Ajusta a configuração de folhas
    nova_raiz->eh_folha = 0;
    if (raiz->filhos[0] == NULL) raiz->eh_folha = 1;
    // Só passar a ser folha se não tiver filhos

    // Pega a chave que vai subir
    int chave_meio = raiz->chaves[1];
    raiz->chaves[1] = -1;
    raiz->qtd_chaves--;

    // Sobe com a chave
    nova_raiz->chaves[0] = chave_meio;
    nova_raiz->qtd_chaves++;

    // Pega a chave que vai pro lado
    int chave_direita = raiz->chaves[2];
    raiz->chaves[2] = -1;
    raiz->qtd_chaves--;

    // Mexe com os filhos do novo nó
    if (raiz->filhos[2] != NULL){
        novo_no->eh_folha = 0;
        novo_no->filhos[0] = raiz->filhos[2];
        novo_no->filhos[1] = raiz->filhos[3];
        raiz->filhos[2] = NULL;
        raiz->filhos[3] = NULL;
    }

    // Joga as chaves pro novo nó se ele for folha
    if (novo_no->eh_folha){
        novo_no->chaves[0] = chave_meio;
        novo_no->qtd_chaves++;
        novo_no->chaves[1] = chave_direita;
        novo_no->qtd_chaves++;
    }
    // Se não for nó, joga só a chave da direita
    else{
        novo_no->chaves[0] = chave_direita;
        novo_no->qtd_chaves++;
    }

    // Ajusta as ligações pai-filho
    nova_raiz->filhos[0] = raiz;
    nova_raiz->filhos[1] = novo_no;

    // Ajusta as ligações de folha
    if (raiz->eh_folha) raiz->prox = novo_no;
    // Só vai criar uma nova ligação se a raiz antiga for folha

    return nova_raiz;
}

void cisao(No* raiz, No* no){
    // Nó que vai ficar "em cima" (já existe)
    No* no_pai = encontraPai(raiz, no);
    // Nó que vai ficar "na direita"
    No* novo_no = criaNo(); 

    // Pega a chave que vai "subir"
    int chave_meio = no->chaves[1];
    no->chaves[1] = -1;
    no->qtd_chaves--;

    // Sobe com a chave
    no_pai->chaves[no_pai->qtd_chaves] = chave_meio;
    ordena(no_pai);
    no_pai->qtd_chaves++;

    // Pega a chave que vai pro lado
    int chave_direita = no->chaves[2];
    no->chaves[2] = -1;
    no->qtd_chaves--;

    // Se o nó que vai sofrer a cisão for folha
    if (no->eh_folha){
        // Joga pro novo nó a chave que subiu
        novo_no->chaves[0] = chave_meio;
        novo_no->qtd_chaves++;
        // Joga a chave que sobrou pro nó novo
        novo_no->chaves[1] = chave_direita;
        novo_no->qtd_chaves++;
        // Ajusta as ligações de folha
        if (no->prox != NULL) novo_no->prox = no->prox;
        no->prox = novo_no;
    }
    else{
        novo_no->eh_folha = 0;
        // Joga pro novo nó a chave que sobrou
        novo_no->chaves[0] = chave_direita;
        novo_no->qtd_chaves++;
        // Jogas os filhos do nó original pro novo nó
        novo_no->filhos[0] = no->filhos[2];
        no->filhos[2] = NULL;
        novo_no->filhos[1] = no->filhos[3];
        no->filhos[3] = NULL;
    }

    // Ajusta as ligações pai-filho
    // Encontra a posição do nó em que houve a cisão
    int pos;
    for(pos=0; pos<no_pai->qtd_chaves; pos++){
        if (no_pai->filhos[pos] == no) break;
    }
    pos++;
    // Move o ponteiro no pai se for necessário
    if (no_pai->filhos[pos] != NULL){
        no_pai->filhos[pos+1] = no_pai->filhos[pos];
        novo_no->prox = no_pai->filhos[pos+1];
    }
    // Atualiza a ligação para o novo nó
    no_pai->filhos[pos] = novo_no;
    ordena(no_pai);
}

No* encontraPai(No* atual, No* filho){

    if (atual->eh_folha == 1){
        return NULL;
    }

    // Procura no nó atual
    for(int i=0; i<QTD_PTRS; i++){
        if(atual->filhos[i] == filho){
            return atual;
        }
    }

    // Aciona uma recursão que vai procurar nos filhos
    for (int i = 0; i < QTD_PTRS; i++) {
        if (atual->filhos[i] != NULL) {
            No* aux = encontraPai(atual->filhos[i], filho);
            if (aux != NULL) {
                return aux;  // Retorna o pai encontrado
            }
        }
    }

    return NULL;
}

No* encontraPaiAntigo(No* raiz, No* filho){ 
    No* atual = raiz;
    int i;
    for(i=0; i<atual->qtd_chaves; i++){
        if (atual->filhos[i] == filho) return atual;
    }
    if (atual->filhos[i] == filho) return atual;
    return NULL;
}

void analiseCisao(Arvore* arvore, No* folha){
    if (folha->qtd_chaves > QTD_CHAVES){
        if (folha == arvore->raiz){
            arvore->raiz = cisaoRaiz(folha);
        }
        else{
            cisao(arvore->raiz, folha);
            No* no_pai = encontraPai(arvore->raiz, folha);
            analiseCisao(arvore, no_pai);
        }
    }
}

void imprimeFolhas(No* atual){
    printf("\n** Impressao das folhas **\n");
    int i, tam;

    while(!atual->eh_folha){
        atual = atual->filhos[0];
    }

    while(atual!=NULL){
        i = 0;
        tam = atual->qtd_chaves;
        printf("\nChaves do no %p ", atual);
        while(i<tam){
            printf("%d ", atual->chaves[i]);
            i++;
        }
        atual = atual->prox;
    }

}

void imprimeChaves(No* no){
    printf("!! Chaves: ");
    for(int i=0; i<no->qtd_chaves; i++){
        printf("%d ", no->chaves[i]);
    }
    printf("!!");
}

void delete_key(Arvore* arvore, int chave) {
    // Encontra a folha que pode conter a chave
    No* folha = encontraFolha(arvore->raiz, chave);
    
    if (!folha) {
        printf("Chave %d nao encontrada.\n", chave);
        return;
    }

    // Verifica se a chave está presente no nó folha
    int i, encontrou = 0;
    for (i = 0; i < folha->qtd_chaves; i++) {
        if (folha->chaves[i] == chave) {
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Chave %d nao encontrada.\n", chave);
        return;
    }

    // Remove a chave do nó
    for (int j = i; j < folha->qtd_chaves - 1; j++) {
        folha->chaves[j] = folha->chaves[j + 1];
    }
    folha->chaves[folha->qtd_chaves - 1] = -1; // Marca o espaço como vazio
    folha->qtd_chaves--;

    // Caso especial: Se a folha for a raiz e ficar vazia
    if (folha == arvore->raiz && folha->qtd_chaves == 0) {
        free(folha);
        arvore->raiz = NULL;
        printf("Arvore esvaziada.\n");
        return;
    }

    // Analisa necessidade de redistribuição ou fusão
    if (folha->qtd_chaves < (QTD_CHAVES + 1) / 2) {
        No* pai = encontraPai(arvore->raiz, folha);

        if (!pai) {
            printf("Erro: No sem pai encontrado.\n");
            return;
        }

        // Procura o nó vizinho para fusão ou redistribuição
        No* vizinho = NULL;
        int pos_vizinho = -1;

        for (i = 0; i <= pai->qtd_chaves; i++) {
            if (pai->filhos[i] == folha) {
                pos_vizinho = i > 0 ? i - 1 : i + 1;
                vizinho = pai->filhos[pos_vizinho];
                break;
            }
        }

        if (!vizinho) {
            printf("Erro: Vizinho nao encontrado.\n");
            return;
        }

        // Fusões
        if (vizinho->qtd_chaves + folha->qtd_chaves <= QTD_CHAVES) {
            // Movendo chaves da folha para o vizinho
            for (i = 0; i < folha->qtd_chaves; i++) {
                vizinho->chaves[vizinho->qtd_chaves] = folha->chaves[i];
                vizinho->qtd_chaves++;
            }

            // Ajusta o encadeamento de folhas, se aplicável
            if (folha->prox != NULL) {
                vizinho->prox = folha->prox;
            }

            // Remove a entrada do pai
            for (i = pos_vizinho; i < pai->qtd_chaves - 1; i++) {
                pai->chaves[i] = pai->chaves[i + 1];
                pai->filhos[i + 1] = pai->filhos[i + 2];
            }
            pai->chaves[pai->qtd_chaves - 1] = -1;
            pai->filhos[pai->qtd_chaves] = NULL;
            pai->qtd_chaves--;

            // Libera memória da folha removida
            free(folha);

            // Caso o pai fique vazio, verifica se é a raiz
            if (pai == arvore->raiz && pai->qtd_chaves == 0) {
                arvore->raiz = vizinho;
                free(pai);
            }
        }
        // Redistribuições
        else {
            if (pos_vizinho < i) { // Vizinho à esquerda
                for (i = folha->qtd_chaves; i > 0; i--) {
                    folha->chaves[i] = folha->chaves[i - 1];
                }
                folha->chaves[0] = vizinho->chaves[vizinho->qtd_chaves - 1];
                folha->qtd_chaves++;
                vizinho->chaves[vizinho->qtd_chaves - 1] = -1;
                vizinho->qtd_chaves--;
                pai->chaves[pos_vizinho] = folha->chaves[0];
            } else { // Vizinho à direita
                folha->chaves[folha->qtd_chaves] = vizinho->chaves[0];
                folha->qtd_chaves++;
                for (i = 0; i < vizinho->qtd_chaves - 1; i++) {
                    vizinho->chaves[i] = vizinho->chaves[i + 1];
                }
                vizinho->chaves[vizinho->qtd_chaves - 1] = -1;
                vizinho->qtd_chaves--;
                pai->chaves[pos_vizinho] = vizinho->chaves[0];
            }
        }
    }

    printf("Chave %d removida.\n", chave);
}

No* buscaChave(Arvore* arvore, int chave) {
    if (!arvore || !arvore->raiz) {
        return NULL; // Árvore vazia
    }

    // Encontra a folha onde a chave pode estar
    No* folha = encontraFolha(arvore->raiz, chave);

    if (!folha) {
        return NULL; // Nenhuma folha encontrada
    }

    // Verifica se a chave está presente na folha encontrada
    for (int i = 0; i < folha->qtd_chaves; i++) {
        if (folha->chaves[i] == chave) {
            return folha; // Retorna o nó contendo a chave
        }
    }

    return NULL; // Chave não encontrada
}

