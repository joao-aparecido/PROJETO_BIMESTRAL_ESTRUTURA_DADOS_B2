#include "cidades.h"

// Função auxiliar para criar um novo nó de cidade
Cidade *criaCidade(const char *nome, int posicao) {
    Cidade *nova = (Cidade *)malloc(sizeof(Cidade));
    if (nova == NULL) {
        perror("Erro ao alocar memória para Cidade");
        return NULL;
    }
    strncpy(nova->Nome, nome, 19);
    nova->Nome[19] = '\0';
    nova->Posicao = posicao;
    nova->Proximo = NULL;
    return nova;
}

// Função auxiliar para inserir uma cidade na lista ligada de forma ordenada por posição
void insereCidadeOrdenada(Estrada *estrada, Cidade *nova) {
    if (estrada->Inicio == NULL || nova->Posicao < estrada->Inicio->Posicao) {
        nova->Proximo = estrada->Inicio;
        estrada->Inicio = nova;
        return;
    }

    Cidade *atual = estrada->Inicio;
    while (atual->Proximo != NULL && atual->Proximo->Posicao < nova->Posicao) {
        atual = atual->Proximo;
    }

    nova->Proximo = atual->Proximo;
    atual->Proximo = nova;
}

// 1. Implementação de getEstrada
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (estrada == NULL) {
        fclose(arquivo);
        return NULL;
    }
    estrada->Inicio = NULL;

    char linha[100];

    // Leitura do comprimento total da estrada (T)
    if (fgets(linha, 100, arquivo) == NULL || sscanf(linha, "%d", &estrada->T) != 1) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Leitura do número de cidades (N)
    if (fgets(linha, 100, arquivo) == NULL || sscanf(linha, "%d", &estrada->N) != 1) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Adiciona as fronteiras
    insereCidadeOrdenada(estrada, criaCidade("Fronteira Oeste", 0));

    // Leitura das cidades
    for (int i = 0; i < estrada->N; i++) {
        if (fgets(linha, 100, arquivo) == NULL) {
            liberaEstrada(estrada);
            fclose(arquivo);
            return NULL;
        }

        int posicao;
        char nome[20];
        if (sscanf(linha, "%d %[^\n]", &posicao, nome) == 2) {
            // Remove o espaço em branco à esquerda do nome, se houver
            char *nome_ptr = nome;
            while (*nome_ptr == ' ') {
                nome_ptr++;
            }
            insereCidadeOrdenada(estrada, criaCidade(nome_ptr, posicao));
        } else {
            liberaEstrada(estrada);
            fclose(arquivo);
            return NULL;
        }
    }

    insereCidadeOrdenada(estrada, criaCidade("Fronteira Leste", estrada->T));

    fclose(arquivo);
    return estrada;
}

// Função auxiliar para liberar a memória da estrutura Estrada
void liberaEstrada(Estrada *estrada) {
    if (estrada == NULL) return;
    Cidade *atual = estrada->Inicio;
    while (atual != NULL) {
        Cidade *proximo = atual->Proximo;
        free(atual);
        atual = proximo;
    }
    free(estrada);
}

// 2. Implementação de calcularMenorVizinhanca
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) return 0.0;

    double menorVizinhanca = -1.0;
    Cidade *atual = estrada->Inicio;

    // A lista ligada tem pelo menos 2 nós (fronteira 0 e fronteira T) se a leitura foi bem sucedida.
    // A vizinhança de uma cidade A é definida como a menor distância entre A e qualquer outra cidade B.
    // O problema pede a menor vizinhança *entre todas as cidades*.
    // A vizinhança de uma cidade é a menor distância até a cidade vizinha mais próxima.
    // A vizinhança da cidade A é min(distância(A, vizinho_anterior), distância(A, vizinho_posterior)).
    // A menor vizinhança da estrada é min(vizinhança(cidade_1), vizinhança(cidade_2), ...).

    // A vizinhança de uma cidade X é a menor distância entre X e qualquer outra cidade Y.
    // Como as cidades estão ordenadas, o vizinho mais próximo de X será X->Proximo ou o nó anterior.
    // A distância entre duas cidades A e B é |Posicao_A - Posicao_B|.

    // Vamos iterar sobre as cidades *reais* (excluindo as fronteiras).
    // As fronteiras (0 e T) não têm vizinhança calculada, pois são apenas limites.
    // A vizinhança de uma cidade A é definida como TODOS os pontos da estrada que são mais próximos do centro da cidade A do que do centro de qualquer outra cidade.
    // O tamanho dessa vizinhança é a distância entre os pontos médios entre A e seus vizinhos.

    // O ponto médio entre A e B é (Posicao_A + Posicao_B) / 2.
    // O tamanho da vizinhança de A é:
    // (Ponto médio entre A e vizinho_posterior) - (Ponto médio entre A e vizinho_anterior)

    // O primeiro nó é a fronteira 0. O último nó é a fronteira T.
    // O loop deve começar na segunda cidade (o primeiro nó real) e ir até a penúltima cidade (o último nó real).

    if (atual != NULL && atual->Proximo != NULL) {
        atual = atual->Proximo; // Começa na primeira cidade real (depois da fronteira 0)
    }

    while (atual != NULL && atual->Proximo != NULL) {
        // atual->Proximo é o vizinho posterior
        // O vizinho anterior é o nó que aponta para 'atual'. Precisamos de um ponteiro para o anterior.
        // Vamos refazer a iteração para ter acesso ao anterior.

        liberaEstrada(estrada);
        // Refaz a leitura para calcular a vizinhança, pois a estrutura de dados é mais complexa.
        // O cálculo da vizinhança é baseado nos pontos médios entre as cidades adjacentes.
        // A vizinhança de uma cidade A é a distância entre o ponto médio de (anterior, A) e o ponto médio de (A, posterior).

        // O ponto médio entre A e B é (Posicao_A + Posicao_B) / 2.
        // O tamanho da vizinhança de A é:
        // ((atual->Proximo->Posicao + atual->Posicao) / 2.0) - ((anterior->Posicao + atual->Posicao) / 2.0)
        // Simplificando: (atual->Proximo->Posicao - anterior->Posicao) / 2.0

        // Vamos refazer a iteração com um ponteiro para o anterior.
        estrada = getEstrada(nomeArquivo);
        if (estrada == NULL) return 0.0;

        menorVizinhanca = -1.0;
        Cidade *anterior = NULL;
        atual = estrada->Inicio;

        while (atual != NULL) {
            if (anterior != NULL && atual->Proximo != NULL) {
                // Estamos em uma cidade real (não fronteira) se anterior e proximo não forem NULL.
                // A vizinhança é calculada para todas as cidades, incluindo as fronteiras.
                // O PDF diz: "A vizinhança da estrada de uma cidade A é definida como todos os pontos da estrada que são mais próximos do centro da cidade A do que do centro de qualquer outra cidade."
                // E o exemplo mostra a vizinhança das cidades reais.

                // Vamos considerar que a vizinhança é calculada apenas para as cidades reais (N cidades).
                // As fronteiras (0 e T) são apenas limites.
                // Se a cidade atual for uma fronteira, seu nome será "Fronteira Oeste" ou "Fronteira Leste".

                if (strcmp(atual->Nome, "Fronteira Oeste") != 0 && strcmp(atual->Nome, "Fronteira Leste") != 0) {
                    // Cidade real
                    double vizinhanca = ((double)atual->Proximo->Posicao - (double)anterior->Posicao) / 2.0;

                    if (menorVizinhanca == -1.0 || vizinhanca < menorVizinhanca) {
                        menorVizinhanca = vizinhanca;
                    }
                }
            }
            anterior = atual;
            atual = atual->Proximo;
        }
    }

    liberaEstrada(estrada);
    return menorVizinhanca;
}

// 3. Implementação de cidadeMenorVizinhanca
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) return NULL;

    double menorVizinhanca = -1.0;
    char *nomeCidade = NULL;

    Cidade *anterior = NULL;
    Cidade *atual = estrada->Inicio;

    while (atual != NULL) {
        if (anterior != NULL && atual->Proximo != NULL) {
            if (strcmp(atual->Nome, "Fronteira Oeste") != 0 && strcmp(atual->Nome, "Fronteira Leste") != 0) {
                double vizinhanca = ((double)atual->Proximo->Posicao - (double)anterior->Posicao) / 2.0;

                if (menorVizinhanca == -1.0 || vizinhanca < menorVizinhanca) {
                    menorVizinhanca = vizinhanca;
                    // Aloca memória para o nome da cidade e copia
                    if (nomeCidade != NULL) {
                        free(nomeCidade);
                    }
                    nomeCidade = strdup(atual->Nome);
                    if (nomeCidade == NULL) {
                        liberaEstrada(estrada);
                        return NULL;
                    }
                }
            }
        }
        anterior = atual;
        atual = atual->Proximo;
    }

    liberaEstrada(estrada);
    return nomeCidade; // O chamador deve liberar esta memória
}
