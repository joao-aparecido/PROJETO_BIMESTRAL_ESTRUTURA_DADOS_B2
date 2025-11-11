#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Função auxiliar para liberar a memória alocada por getEstrada
void liberaEstrada(Estrada *estrada);

int main() {
    int NumTeste = 0;
    FILE *Resposta = fopen("Resultado.txt", "w");
    if (Resposta == NULL) {
        perror("Erro ao abrir Resultado.txt");
        return 1;
    }

    // Teste 1: getEstrada
    Estrada *T1 = getEstrada("teste01.txt");
    if (T1 != NULL) {
        fprintf(Resposta, "Teste getEstrada (teste01.txt): SUCESSO\n");
        NumTeste++;
        // Verificação adicional (opcional):
        // if (T1->T == 10 && T1->N == 2) Soma++;
        // NumTeste++;
    } else {
        fprintf(Resposta, "Teste getEstrada (teste01.txt): FALHA\n");
    }
    liberaEstrada(T1); // Libera a memória após o teste

    // Teste 2: calcularMenorVizinhanca
    double D1 = calcularMenorVizinhanca("teste01.txt");
    double Soma = 0;
    if (D1 > 0) { // Verifica se o cálculo foi realizado
        NumTeste++;
        if (D1 == 3.5) {
            Soma += 1.5; // Exemplo do PDF sugere 1.5 pontos para este teste
        }
    }

    // Teste 3: cidadeMenorVizinhanca
    char *C1 = cidadeMenorVizinhanca("teste01.txt");
    if (C1 != NULL) {
        NumTeste++;
        if (strcmp(C1, "Birnin Zana") == 0) {
            Soma += 1.5; // Exemplo do PDF sugere 1.5 pontos para este teste
        }
        free(C1); // Libera a memória alocada pela função
    }

    // Teste 4: Teste com o segundo arquivo de exemplo (teste02.txt)
    // Esperado: menor vizinhança = 3.5 (L'pur), cidade = L'pur
    double D2 = calcularMenorVizinhanca("teste02.txt");
    if (D2 > 0) {
        NumTeste++;
        if (D2 == 3.5) {
            Soma += 1.5;
        }
    }

    char *C2 = cidadeMenorVizinhanca("teste02.txt");
    if (C2 != NULL) {
        NumTeste++;
        if (strcmp(C2, "L'pur") == 0) {
            Soma += 1.5;
        }
        free(C2);
    }

    // O PDF original tinha um trecho de código de teste mais complexo.
    // Vou simplificar a saída para apenas indicar o sucesso da execução e os resultados.

    fprintf(Resposta, "\n--- Resultados dos Testes ---\n");
    fprintf(Resposta, "Menor Vizinhança (teste01.txt): %.2f (Esperado: 3.50)\n", D1);
    fprintf(Resposta, "Cidade com Menor Vizinhança (teste01.txt): %s (Esperado: Birnin Zana)\n", C1 != NULL ? C1 : "N/A");
    fprintf(Resposta, "Menor Vizinhança (teste02.txt): %.2f (Esperado: 3.50)\n", D2);
    fprintf(Resposta, "Cidade com Menor Vizinhança (teste02.txt): %s (Esperado: L'pur)\n", C2 != NULL ? C2 : "N/A");
    fprintf(Resposta, "\nTotal de Testes Executados: %d\n", NumTeste);
    fprintf(Resposta, "Pontuação Estimada (apenas para referência): %.2f\n", Soma);

    fclose(Resposta);
    return 0;
}


