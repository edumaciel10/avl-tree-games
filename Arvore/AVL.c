#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "AVL.h"

struct node_st
{
    JOGO *jogo;
    NODE *direita;
    NODE *esquerda;
    int altura;
};

struct avl_st
{
    NODE* raiz;
    int profundidade;
};

// Criação
NODE *avl_cria_no(JOGO *jogo);

// Inserção
static NODE *avl_inserir_no_e_rotacionar(NODE *raiz, JOGO *jogo);
static NODE *inserir_no(NODE *raiz, JOGO *jogo);

// Rotação
static NODE *selecionar_e_executar_rotacao(NODE *raiz, JOGO *jogo);
NODE *rodar_esquerda(NODE *a);
NODE *rodar_direita(NODE *a);
NODE *rodar_esquerda_direita(NODE *a);
NODE *rodar_direita_esquerda(NODE *a);

// Remoção
static NODE* avl_remover_no_e_rotacionar(NODE **raiz, JOGO *jogo);
static NODE* remover_no(NODE **raiz, JOGO *jogo);
static void troca_max_esq(NODE **troca, NODE *raiz, NODE *ant);

// Ordem
static void pre_ordem_recursivo(NODE *no);
static void em_ordem_recursivo(NODE *no);
static void pos_ordem_recursivo(NODE *no);

// Apagar árvore
static void avl_apagar_recursivo(NODE **raiz);
static void apagar_no(NODE **no);

// Auxiliares
int avl_altura_no(NODE* raiz);
static bool JogoEhMenor(const NODE *raiz, JOGO *jogo);
static bool JogoEhMaior(const NODE *raiz, JOGO *jogo);
static bool DesbalanceamentoEhPositivo(NODE *raiz);
static bool DesbalanceamentoEhNegativo(NODE *raiz);
static int calcula_balanceamento(NODE *raiz);
static int contar_nos_por_ano(NODE *raiz, JOGO *jogo);

// Funções de criação
AVL *avl_criar() {
    AVL *arvore = (AVL *) malloc(sizeof(AVL));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->profundidade = -1;
    }
    return arvore;
}

NODE *avl_cria_no(JOGO *jogo) {
    NODE *no = (NODE *) malloc(sizeof (NODE));
    if (no != NULL) {
        no->altura = 0;
        no->direita = NULL;
        no->esquerda = NULL;
        no->jogo = jogo;
    }
    return no;
}

// Funções de inserção
boolean avl_inserir(AVL *arvore, JOGO *jogo) {
    arvore->raiz = avl_inserir_no_e_rotacionar(arvore->raiz, jogo);
    return arvore->raiz != NULL;
}

static NODE *avl_inserir_no_e_rotacionar(NODE *raiz, JOGO *jogo) {
    raiz = inserir_no(raiz, jogo);

    raiz->altura = max(avl_altura_no(raiz->esquerda),
                       avl_altura_no(raiz->direita)) + 1;

    raiz = selecionar_e_executar_rotacao(raiz, jogo);

    return raiz;
}

static NODE *inserir_no(NODE *raiz, JOGO *jogo) {
    if (raiz == NULL) {
        raiz = avl_cria_no(jogo);
    }
    else if (JogoEhMaior(raiz, jogo)) {
        raiz->direita = avl_inserir_no_e_rotacionar(raiz->direita, jogo);
    }
    else if (JogoEhMenor(raiz, jogo)) {
        raiz->esquerda = avl_inserir_no_e_rotacionar(raiz->esquerda, jogo);
    }
    return raiz;
}

// Funções de rotação
static NODE *selecionar_e_executar_rotacao(NODE *raiz, JOGO *jogo) {
    if (DesbalanceamentoEhNegativo(raiz)) {
        if (calcula_balanceamento(raiz->direita) < 0) {
            raiz = rodar_esquerda(raiz);
        }
        else {
            raiz = rodar_direita_esquerda(raiz);
        }
    }

    if (DesbalanceamentoEhPositivo(raiz)) {
        if (calcula_balanceamento(raiz->esquerda) > 0) {
            raiz = rodar_direita(raiz);
        }
        else{
            raiz = rodar_esquerda_direita(raiz);
        }
    }
    return raiz;
}

NODE *rodar_esquerda(NODE *a) {
    NODE *b = a->direita;

    if(b == NULL){
        return a;
    }

    a->direita = b->esquerda;
    b->esquerda = a;

    a->altura = max(avl_altura_no(a->esquerda),
                    avl_altura_no(a->direita)) + 1;
    b->altura = max(avl_altura_no(b->direita),
                    a->altura) + 1;
    return b;
}

NODE *rodar_direita(NODE *a) {
    NODE *b = a->esquerda;

    if(b == NULL){
        return a;
    }
    
    a->esquerda = b->direita;
    b->direita = a;

    a->altura = max(avl_altura_no(a->esquerda),
                    avl_altura_no(a->direita)) + 1;
    b->altura = max(avl_altura_no(b->esquerda),
                    a->altura) + 1;
    return b;
}

NODE *rodar_esquerda_direita(NODE *a)   {
    a->esquerda = rodar_esquerda(a->esquerda);
    return rodar_direita(a);
}

NODE *rodar_direita_esquerda(NODE *a)   {
    a->direita = rodar_direita(a->direita);
    return rodar_esquerda(a);
}

// Funções de remoção
int avl_remover_por_ano(AVL *arvore, int ano) {
    JOGO *jogoTemporario = jogo_criar_vazio();
    if(jogoTemporario == NULL){
        return -1;
    }
    jogo_set_ano(jogoTemporario, ano);

    int jogosParaRemover = contar_nos_por_ano(arvore->raiz, jogoTemporario);

    for(int i = 0; i < jogosParaRemover; i++){
        avl_remover_no_e_rotacionar(&(arvore->raiz), jogoTemporario);
    }
    jogo_apagar(&jogoTemporario);

    return jogosParaRemover;
}

static NODE* avl_remover_no_e_rotacionar(NODE **raiz, JOGO *jogo) {
    (*raiz) = remover_no(raiz, jogo);

    if(*raiz == NULL){
        return *raiz;
    }

    (*raiz)->altura = max(avl_altura_no((*raiz)->esquerda),
                    avl_altura_no((*raiz)->direita)) + 1;

    (*raiz) = selecionar_e_executar_rotacao(*raiz, jogo);

    return (*raiz);
}

static NODE* remover_no(NODE **raiz, JOGO *jogo) {
    NODE *noRemovido;

    if (*raiz == NULL) {
        return *raiz;
    }
    if(jogo_get_ano((*raiz)->jogo) == jogo_get_ano(jogo)){
        if((*raiz)->esquerda == NULL || (*raiz)->direita == NULL)
        {
            noRemovido = *raiz;
            if((*raiz)->esquerda == NULL) {
                *raiz = (*raiz)->direita;
            }
            else {
                *raiz = (*raiz)->esquerda;
            }
            apagar_no(&noRemovido);
        }else{
            troca_max_esq(&(*raiz)->esquerda, *raiz, *raiz);
        }
    }
    else if (JogoEhMaior(*raiz, jogo)) {
        (*raiz)->direita = avl_remover_no_e_rotacionar(&(*raiz)->direita, jogo);
    }
    else if (JogoEhMenor(*raiz, jogo)) {
        (*raiz)->esquerda = avl_remover_no_e_rotacionar(&(*raiz)->esquerda, jogo);
    }

    return (*raiz);
}
static void troca_max_esq(NODE **troca, NODE *raiz, NODE *ant)
{
    if((*troca)->direita != NULL)
    {
        troca_max_esq(&(*troca)->direita, raiz, *troca);
        return;
    }

    NODE* trocaReserva = (*troca);
    if(raiz == ant) {
        ant->esquerda = (*troca)->esquerda;
    }
    else {
        ant->direita = (*troca)->esquerda;
    }
    jogo_apagar(&(raiz->jogo));
    raiz->jogo = trocaReserva->jogo;
    free(trocaReserva);
    trocaReserva = NULL;
}
// Funções de ordem
void avl_pre_ordem(AVL *arvore){
    pre_ordem_recursivo(arvore->raiz);
}

static void pre_ordem_recursivo(NODE *no){
    if (no != NULL) {
        printf("%s\n", jogo_get_nome(no->jogo));
        pre_ordem_recursivo(no->esquerda);
        pre_ordem_recursivo(no->direita);
    }
}

void avl_em_ordem(AVL *arvore){
    em_ordem_recursivo(arvore->raiz);
}

static void em_ordem_recursivo(NODE *no){
    if (no != NULL) {
        em_ordem_recursivo(no->esquerda);
        printf("%s\n", jogo_get_nome(no->jogo));
        em_ordem_recursivo(no->direita);
    }
}

void avl_pos_ordem(AVL *arvore){
    pos_ordem_recursivo(arvore->raiz);
}

static void pos_ordem_recursivo(NODE *no){
    if (no != NULL) {
        pos_ordem_recursivo(no->esquerda);
        pos_ordem_recursivo(no->direita);
        printf("%s\n", jogo_get_nome(no->jogo));
    }
}

// Funções para apagar a árvore
void avl_apagar(AVL **arvore) {
    avl_apagar_recursivo(&(*arvore)->raiz);
    free(*arvore);
    *arvore = NULL;
}

static void avl_apagar_recursivo(NODE **raiz) {
    if (*raiz != NULL) {
        avl_apagar_recursivo(&((*raiz)->esquerda));
        avl_apagar_recursivo(&((*raiz)->direita));
        apagar_no(raiz);
    }
}

static void apagar_no(NODE **no){
    jogo_apagar(&(*no)->jogo);
    free(*no);
    *no = NULL;
}

// Funções auxiliares
int avl_altura_no(NODE* raiz) {
    if (raiz == NULL) {
        return -1;
    } else {
        return raiz->altura;
    }
}

static bool JogoEhMenor(const NODE *raiz, JOGO *jogo) {
    int anoJogoAtual = jogo_get_ano(jogo);
    int anoJogoRaiz = jogo_get_ano(raiz->jogo);

    if(anoJogoAtual < anoJogoRaiz){
        return TRUE;
    }

    else if(anoJogoAtual > anoJogoRaiz){
        return FALSE;
    }

    else{
        char* nomeJogoAtual = jogo_get_nome(jogo);
        char* nomeJogoRaiz = jogo_get_nome(raiz->jogo);

        return (strcmp(nomeJogoAtual, nomeJogoRaiz) < 0); 
    }
}

static bool JogoEhMaior(const NODE *raiz, JOGO *jogo) {
    int anoJogoAtual = jogo_get_ano(jogo);
    int anoJogoRaiz = jogo_get_ano(raiz->jogo);

    if(anoJogoAtual > anoJogoRaiz){
        return TRUE;
    }

    else if(anoJogoAtual < anoJogoRaiz){
        return FALSE;
    }

    else{
        char* nomeJogoAtual = jogo_get_nome(jogo);
        char* nomeJogoRaiz = jogo_get_nome(raiz->jogo);

        return (strcmp(nomeJogoAtual, nomeJogoRaiz) > 0); 
    }
}

static bool DesbalanceamentoEhPositivo(NODE *raiz) {
    return calcula_balanceamento(raiz) == 2;
}

static bool DesbalanceamentoEhNegativo(NODE *raiz) {
    return calcula_balanceamento(raiz) == -2;
}

static int calcula_balanceamento(NODE *raiz){
    return avl_altura_no(raiz->esquerda) - avl_altura_no(raiz->direita);
}

static int contar_nos_por_ano(NODE *raiz, JOGO *jogo) {
    if (raiz == NULL) {
        return 0;
    }

    int contagemDosFilhos = contar_nos_por_ano(raiz->esquerda, jogo) + contar_nos_por_ano(raiz->direita, jogo);

    if(jogo_get_ano(raiz->jogo) == jogo_get_ano(jogo)){
        return 1 + contagemDosFilhos;
    }

    return contagemDosFilhos;
}
AVL *lerJogosDoCSV(){
    FILE *arquivo;
    arquivo = fopen("CSV.csv", "r");
    fseek(arquivo, 3, SEEK_SET);
    JOGO *jogo;
    int key = 1;
    AVL *arvore = avl_criar();
    while( !feof(arquivo) ){
        jogo = jogo_ler_linha_csv(arquivo, key);
        if(jogo != NULL){
            avl_inserir(arvore, jogo);
            key++;
        }
    }
    fclose(arquivo);
    return arvore;
}
