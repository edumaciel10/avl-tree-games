#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Jogo.h"

struct game_st
{
    int chave;
    char nome[50];
    int ano;
    char empresa[50];
};

JOGO *jogo_criar_vazio()
{
    JOGO *jogo;

    jogo = (JOGO *)calloc(sizeof(JOGO), 1);
    return jogo;
}

JOGO *jogo_criar(int chave, const char *nome, int ano, const char *empresa)
{
    JOGO *jogo;

    jogo = (JOGO *)malloc(sizeof(JOGO));

    if (jogo != NULL)
    {
        jogo->chave = chave;
        strcpy(jogo->nome, nome);
        jogo->ano = ano;
        strcpy(jogo->empresa, empresa);
        return jogo;
    }
    return NULL;
}

JOGO *jogo_ler_linha_csv(FILE *arquivo, int chave){
    char input[50] = "";
    fscanf(arquivo, " %[^(\r|\n)]*c", input);

    if(strstr(input, ";") == NULL){
        return NULL;
    }

    char nome[50]; char string_ano[10]; int ano; char empresa[50];

    char *pointer = strtok(input, ";");
    strcpy(nome, pointer);

    pointer = strtok(NULL, ";");
    strcpy(string_ano, pointer);
    ano = atoi(string_ano);

    pointer = strtok(NULL, "\n");
    strcpy(empresa, pointer);
    return jogo_criar(chave, nome, ano, empresa);
}

boolean jogo_apagar(JOGO **jogo)
{
    if (*jogo != NULL)
    {
        (*jogo)->chave = ERRO; /*apaga o jogo simbolicamente*/
        free(*jogo);
        *jogo = NULL;
        return TRUE;
    }
    return FALSE;
}

int jogo_get_chave(const JOGO *jogo)
{
    if (jogo != NULL)
    {
        return jogo->chave;
    }
    return ERRO;
}

boolean jogo_set_chave(JOGO *jogo, int chave)
{
    if (jogo != NULL)
    {
        jogo->chave = chave;
        return TRUE;
    }
    return FALSE;
}

char *jogo_get_nome(JOGO *jogo)
{
    if(jogo == NULL){
        return NULL;
    }

    return jogo->nome;
}

int jogo_get_ano(JOGO *jogo)
{
    if(jogo == NULL){
        return -1;
    }

    return jogo->ano;
}

char *jogo_get_empresa(JOGO *jogo)
{
    if(jogo == NULL){
        return NULL;
    }

    return jogo->empresa;
}

boolean jogos_iguais(const JOGO *jogo1, const JOGO *jogo2)
{
    boolean iguais = FALSE;
    if (jogo1 != NULL && jogo2 != NULL)
    {
        if(jogo1->ano == jogo2->ano)
        {
            if(strcmp(jogo1->nome, jogo2->nome) == 0)
            {
                if(strcmp(jogo1->empresa, jogo2->empresa) == 0)
                {
                    iguais = TRUE;
                }
            }
        }
    }
    return iguais;
}
