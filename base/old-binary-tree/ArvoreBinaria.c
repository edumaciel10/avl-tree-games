#include <stdlib.h>
#include "ArvoreBinaria.h"
#include <stdio.h>


struct node_st
{
    CLIENTE *cliente;
    NODE *direita;
    NODE *esquerda;
};

struct arvore_binaria
{
    NODE* raiz;
};

ARVORE_BINARIA *arvore_binaria_criar()
{
    ARVORE_BINARIA *arvore;
    arvore = (ARVORE_BINARIA *) malloc(sizeof(ARVORE_BINARIA));
    if (arvore != NULL) {
        arvore->raiz = NULL;
    }
    return arvore;
}

static void pre_ordem_recursao(NODE *raiz) {
    if (raiz != NULL) {
        cliente_imprimir(raiz->cliente);
        pre_ordem_recursao(raiz->esquerda);
        pre_ordem_recursao(raiz->direita);
    }
}

void arvore_binaria_pre_ordem(ARVORE_BINARIA *arvoreBinaria)
{
    printf("Preorder\n");
    pre_ordem_recursao(arvoreBinaria->raiz);
}

NODE * ab_cria_no(CLIENTE *cliente)
{
    NODE * novo_no;
    novo_no = (NODE *) malloc(sizeof(NODE));
    if (novo_no != NULL) {
        novo_no->cliente = cliente;
        novo_no->direita = NULL;
        novo_no->esquerda = NULL;
    }
    return novo_no;
}
boolean no_checa_duplicado(NODE *raiz, CLIENTE *cliente)
{
    if (cliente_comparar(raiz->cliente, cliente) == 0) {
        return TRUE;
    }
    if(raiz->esquerda != NULL) {
        if (cliente_comparar(raiz->esquerda->cliente, cliente) == 0) {
            return TRUE;
        }
    }
    if(raiz->direita != NULL) {
        if (cliente_comparar(raiz->direita->cliente, cliente) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

NODE * ab_inserir_no(NODE* raiz, CLIENTE *cliente)
{
    if (raiz == NULL) {
        raiz = ab_cria_no(cliente);
    } else {
        if (cliente_comparar(cliente, raiz->cliente) < 0) {
            raiz->esquerda = ab_inserir_no(raiz->esquerda, cliente);
        } else if (cliente_comparar(cliente, raiz->cliente) > 0) {
            raiz->direita = ab_inserir_no(raiz->direita, cliente);
        } else {
            return NULL;
        }
    }
    return raiz;
}


boolean ab_inserir(ARVORE_BINARIA *T, CLIENTE *cliente){
    if (T->raiz == NULL)
        return((T->raiz = ab_cria_no(cliente)) != NULL);
    else
        return((T->raiz = ab_inserir_no(T->raiz, cliente)) != NULL);
}

boolean ab_apagar_no(NODE *raiz) {
    if (raiz == NULL) {
        return FALSE;
    }
    ab_apagar_no(raiz->esquerda);
    ab_apagar_no(raiz->direita);
    cliente_apagar(&(raiz->cliente));
    free(raiz);
    raiz = NULL;
    return TRUE;
}
boolean ab_apagar (ARVORE_BINARIA **T) {
    if (*T == NULL) {
        return FALSE;
    }
    ab_apagar_no((*T)->raiz);
    free(*T);
    *T = NULL;
    return TRUE;
}


void arvore_binaria_buscar(ARVORE_BINARIA *arvore, char* CPF) {
    NODE *atual = arvore->raiz;
    int encontrou = FALSE;
    long chave = cliente_ler_chave_input(CPF);
    while (atual != NULL) {
        // cliente_imprimir(atual->cliente);
        // printf("cliente_comparar(atual->cliente, cliente) == %d\n", cliente_comparar_chave(atual->cliente, chave));
        if (cliente_comparar_chave(atual->cliente, chave) == 0) {
            encontrou = TRUE;
            break;
        }
        if (cliente_comparar_chave(atual->cliente, chave) > 0) { // refatorar isso
            atual = atual->esquerda;
        } else if(cliente_comparar_chave(atual->cliente, chave) < 0) {
            atual = atual->direita;
        }
    }
    if ( encontrou == TRUE ) {
        cliente_imprimir_verboso(atual->cliente);
    } else {
        printf("Cliente não encontrado");
    }
}

void arvore_binaria_deletar(ARVORE_BINARIA *arvore, char* CPF) {
    NODE *atual = arvore->raiz;
    NODE *pai = NULL;
    int encontrou = FALSE;
    long chave = cliente_ler_chave_input(CPF);
    while (atual != NULL) {
        if (cliente_comparar_chave(atual->cliente, chave) == 0) {
            encontrou = TRUE;
            break;
        }
        pai = atual;
        if (cliente_comparar_chave(atual->cliente, chave) > 0) { // refatorar isso
            atual = atual->esquerda;
        } else if(cliente_comparar_chave(atual->cliente, chave) < 0) {
            atual = atual->direita;
        }
    }
    if ( encontrou == TRUE ) {
        cliente_imprimir_verboso(atual->cliente);
        if (atual->esquerda == NULL && atual->direita == NULL) {
            if (pai == NULL) {
                arvore->raiz = NULL;
            } else {
                if (pai->esquerda == atual) {
                    pai->esquerda = NULL;
                } else {
                    pai->direita = NULL;
                }
            }
        } else if (atual->esquerda == NULL) {
            if (pai == NULL) {
                arvore->raiz = atual->direita;
            } else {
                if (pai->esquerda == atual) {
                    pai->esquerda = atual->direita;
                } else {
                    pai->direita = atual->direita;
                }
            }
        } else if (atual->direita == NULL) {
            if (pai == NULL) {
                arvore->raiz = atual->esquerda;
            } else {
                if (pai->esquerda == atual) {
                    pai->esquerda = atual->esquerda;
                } else {
                    pai->direita = atual->esquerda;
                }
            }
        } else {
            NODE *sucessor = atual->direita;
            NODE *sucessor_pai = atual;
            while (sucessor->esquerda != NULL) {
                sucessor_pai = sucessor;
                sucessor = sucessor->esquerda;
            }
            atual->cliente = sucessor->cliente;
            if (sucessor_pai->esquerda == sucessor) {
                sucessor_pai->esquerda = sucessor->direita;
            } else {
                sucessor_pai->direita = sucessor->direita;
            }
        }
        arvore_binaria_pre_ordem(arvore);
    } else {
        printf("Cliente não encontrado");
        return;
    }
    ab_apagar_no(pai);
}