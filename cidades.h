#ifndef CIDADES_H
#define CIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Definição da estrutura da cidade
typedef struct cidade {
    char Nome[20]; // Nome da cidade
    int Posicao; // Posição (coordenada) da cidade
    struct cidade *Proximo; // Ponteiro para a próxima cidade da estrada
} Cidade;

// Definição da estrutura da estrada
typedef struct {
    int N; // Número de cidades
    int T; // Comprimento da estrada
    Cidade *Inicio; // Ponteiro para a primeira cidade da estrada
} Estrada;

// Protótipos das funções
Estrada *getEstrada(const char *nomeArquivo);
double calcularMenorVizinhanca(const char *nomeArquivo);
char *cidadeMenorVizinhanca(const char *nomeArquivo);
void liberaEstrada(Estrada *estrada);

#endif
