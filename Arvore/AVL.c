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

// Ordem
static void pre_ordem_recursivo(NODE *no);
static void em_ordem_recursivo(NODE *no);
static void pos_ordem_recursivo(NODE *no);

// Apagar árvore
static void avl_apagar_recursivo(NODE **raiz);

// Auxiliares
int avl_altura_no(NODE* raiz);
static bool JogoEhMenor(const NODE *raiz, JOGO *jogo);
static bool JogoEhMaior(const NODE *raiz, JOGO *jogo);
static bool DesbalanceamentoEhPositivo(NODE *raiz);
static bool DesbalanceamentoEhNegativo(NODE *raiz);



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
        if (JogoEhMaior(raiz->direita, jogo)) {
            raiz = rodar_esquerda(raiz);
        }
        else {
            raiz = rodar_direita_esquerda(raiz);
        }
    }

    if (DesbalanceamentoEhPositivo(raiz)) {
        if (JogoEhMenor(raiz->esquerda, jogo))
            raiz = rodar_direita(raiz);
        else
            raiz = rodar_esquerda_direita(raiz);
    }
    return raiz;
}

NODE *rodar_esquerda(NODE *a) {
    NODE *b = a->direita;
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
boolean avl_remover_por_ano(AVL *arvore, const int ano) {
    printf("Removendo jogos do ano %d\n", ano);
    return TRUE;
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
        jogo_apagar(&(*raiz)->jogo);
        free(*raiz);
    }
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
    return avl_altura_no(raiz->esquerda)
               - avl_altura_no(raiz->direita) == 2;
}

static bool DesbalanceamentoEhNegativo(NODE *raiz) {
    return avl_altura_no(raiz->esquerda)
               - avl_altura_no(raiz->direita) == -2;
}
