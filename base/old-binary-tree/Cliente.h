#ifndef TADGENERICO_cliente_H
#define TADGENERICO_cliente_H

#include "Util.h"

#define ERRO (-32000)
#define TRUE 1
#define FALSE 0
typedef int boolean;

typedef struct cliente_st CLIENTE;

CLIENTE *cliente_criar_vazio();
CLIENTE *cliente_criar (long chave, char *nome, int idade, float saldo);
boolean cliente_apagar(CLIENTE **cliente);
CLIENTE *cliente_ler_stdin();
void cliente_imprimir(const CLIENTE *cliente);
int cliente_get_chave(const CLIENTE *cliente);
int cliente_comparar(const CLIENTE *cliente1, const CLIENTE *cliente2);
long cliente_ler_chave_input(char chave[13]);
void cliente_imprimir_verboso(const CLIENTE *cliente);
int cliente_comparar_chave(const CLIENTE *cliente, long chave);
char *cliente_ler_cpf();
#endif //TADGENERICO_cliente_H
