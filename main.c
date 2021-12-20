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
        int removidos = avl_remover_por_ano(catalogo, ano);

        printf("Foram removidos %d jogos\n", removidos);

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