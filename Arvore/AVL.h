#ifndef AVL_H
#define AVL_H

#include "../Util/Util.h"
#include "../Jogo/Jogo.h"

typedef int boolean;

#define max(a, b) ((a > b) ? a : b)
typedef struct avl_st AVL;
typedef struct node_st NODE;

#define AVL_PRE_ORDEM 1
#define AVL_EM_ORDEM 2
#define AVL_POS_ORDEM 3

AVL *avl_criar();
boolean avl_inserir(AVL *arvore, JOGO *jogo);
void avl_pre_ordem(AVL *arvore);
void avl_em_ordem(AVL *arvore);
void avl_pos_ordem(AVL *arvore);
boolean avl_remover_por_ano(AVL *arvore, const int ano);
void avl_apagar(AVL **arvore);

#endif //AVL_H
