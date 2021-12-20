#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arvore/AVL.h"

AVL *lerJogosDoCSV();

int main()
{
    AVL *catalogo;
    catalogo = lerJogosDoCSV();

    int ordem;
    scanf("%d", &ordem);
    getchar();

    char *input = readLine();

    int ano;
    while(strcmp(input,"f") != 0) {
        ano = atoi(input);
        avl_remover_por_ano(catalogo, ano);

        free(input);
        input = readLine();
    }
    free(input);

    switch(ordem){
        case AVL_PRE_ORDEM:
            avl_pre_ordem(catalogo);
            break;

        case AVL_EM_ORDEM:
            avl_em_ordem(catalogo);
            break;

        case AVL_POS_ORDEM:
            avl_pos_ordem(catalogo);
            break;
    }

    avl_apagar(&catalogo);
    return 0;
}
