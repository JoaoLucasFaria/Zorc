#include <stdio.h>
#include <stdlib.h>
#include "dinamica.h"

// Estrutura para armazenar o melhor resultado
typedef struct
{
    int habilidade;
    int caminho_tamanho;
    int *caminho;
    int *recrutados;
} Resultado;

// Aloca memória para a tabela de poda e inicializa com -1
int ***aloca_memo(int P, int W, int D)
{
    int ***memo = malloc(P * sizeof(int **));
    for (int i = 0; i < P; i++)
    {
        memo[i] = malloc((W + 1) * sizeof(int *));
        for (int j = 0; j <= W; j++)
        {
            memo[i][j] = malloc((D + 1) * sizeof(int));
            for (int k = 0; k <= D; k++)
            {
                memo[i][j][k] = -1;
            }
        }
    }
    return memo;
}

// Libera memória da tabela de poda
void libera_memo(int ***memo, int P, int W)
{
    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j <= W; j++)
        {
            free(memo[i][j]);
        }
        free(memo[i]);
    }
    free(memo);
}

// Função recursiva com poda
void busca(Povo *grafo, Soldado *soldados, int *visitado,
           int pos, int P, int W, int D,
           int peso, int dist, int habilidade,
           int *caminho, int *recrutados, int tam,
           Resultado *melhor, int ***memo)
{

    // Poda por limites
    if (dist > D || peso > W)
        return;

    // Poda: evita recalcular estados piores
    if (memo[pos][peso][dist] != -1 && memo[pos][peso][dist] >= habilidade)
        return;
    memo[pos][peso][dist] = habilidade;

    visitado[pos] = 1;

    int max_s = (soldados[pos].peso == 0) ? 0 : (W - peso) / soldados[pos].peso;

    for (int qtd = 0; qtd <= max_s; qtd++)
    {
        int hab = habilidade + qtd * soldados[pos].habilidade;
        int pes = peso + qtd * soldados[pos].peso;

        caminho[tam] = pos + 1;
        recrutados[tam] = qtd;

        if (hab > melhor->habilidade)
        {
            melhor->habilidade = hab;
            melhor->caminho_tamanho = tam + 1;
            for (int i = 0; i <= tam; i++)
            {
                melhor->caminho[i] = caminho[i];
                melhor->recrutados[i] = recrutados[i];
            }
        }

        for (int i = 0; i < grafo[pos].qtd_vizinhos; i++)
        {
            int viz = grafo[pos].vizinhos[i].destino;
            int dist_viz = grafo[pos].vizinhos[i].distancia;

            if (!visitado[viz])
            {
                busca(grafo, soldados, visitado, viz, P, W, D,
                      pes, dist + dist_viz, hab,
                      caminho, recrutados, tam + 1,
                      melhor, memo);
            }
        }
    }

    visitado[pos] = 0;
}

// Função principal da estratégia ótima
void estrategia_otima(Povo *grafo, Soldado *soldados, int P, int W, int D, FILE *out)
{
    Resultado melhor;
    melhor.habilidade = 0;
    melhor.caminho = malloc(P * sizeof(int));
    melhor.recrutados = malloc(P * sizeof(int));
    melhor.caminho_tamanho = 0;

    int *visitado = calloc(P, sizeof(int));
    int *caminho = malloc(P * sizeof(int));
    int *recrutados = malloc(P * sizeof(int));
    int ***memo = aloca_memo(P, W, D);

    for (int inicio = 0; inicio < P; inicio++)
    {
        busca(grafo, soldados, visitado, inicio, P, W, D, 0, 0, 0, caminho, recrutados, 0, &melhor, memo);
    }

    // Encontra o primeiro índice com recruta > 0
    int primeiro_util = 0;
    while (primeiro_util < melhor.caminho_tamanho && melhor.recrutados[primeiro_util] == 0)
    {
        primeiro_util++;
    }

    // Imprime apenas os povos do primeiro útil em diante
    fprintf(out, "%d", melhor.habilidade);
    for (int i = primeiro_util; i < melhor.caminho_tamanho; i++)
    {
        fprintf(out, " %d %d", melhor.caminho[i], melhor.recrutados[i]);
    }
    fprintf(out, "\n");

    free(melhor.caminho);
    free(melhor.recrutados);
    free(visitado);
    free(caminho);
    free(recrutados);
    libera_memo(memo, P, W);
}