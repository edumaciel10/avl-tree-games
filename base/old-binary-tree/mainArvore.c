#include <stdio.h>
#include <stdlib.h>
#include "ArvoreBinaria.h"
#include "string.h"
int main()
{
    int quantidade;
    ARVORE_BINARIA *arvoreBinaria;
    arvoreBinaria = arvore_binaria_criar();
    CLIENTE *cliente;
    scanf("%d",&quantidade);
    getchar();
    for(int i = 0; i < quantidade; i++)
    {
        cliente = cliente_ler_stdin();
        ab_inserir(arvoreBinaria, cliente);
    }
    // get a char value on variable k
    char *k = readLine();

    if(strcmp(k,"I") ==0) {
        cliente = cliente_ler_stdin();
        ab_inserir(arvoreBinaria, cliente);
        arvore_binaria_pre_ordem(arvoreBinaria);
    }else if(strcmp(k,"R") ==0) {
        char *cpf = readLine();
        arvore_binaria_deletar(arvoreBinaria, cpf);
    }else if(strcmp(k,"B") ==0) {
        char *CPF = readLine();
        arvore_binaria_buscar(arvoreBinaria, CPF);
    }
    printf("\n");
    ab_apagar(&arvoreBinaria);
    return 0;
}