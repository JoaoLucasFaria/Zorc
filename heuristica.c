#include <stdio.h>
#include <stdlib.h>
#include "heuristica.h"

// Verifica se povo V já foi visitado
int ja_foi(int *visitado, int tam, int v)
{
    for (int i = 0; i < tam; i++)
    {
        if (visitado[i] == v)
            return 1;
    }
    return 0;
}

// Estratégia heurística: aproximação gulosa baseada na eficiência (habilidade/peso)
void estrategia_heuristica(Povo *grafo, Soldado *soldados, int P, int W, int D, FILE *out)
{
    int peso_total = 0;
    int habilidade_total = 0;
    int distancia_total = 0;

    int *visitado = malloc(P * sizeof(int));
    int *recrutados = malloc(P * sizeof(int));
    int visitados = 0;

    for (int i = 0; i < P; i++)
    {
        visitado[i] = -1;
        recrutados[i] = 0;
    }

    // Escolhe o melhor povo para começar (maior eficiência habilidade/peso)
    int atual = -1;
    double melhor_ratio = -1;
    for (int i = 0; i < P; i++)
    {
        double ratio = (double)soldados[i].habilidade / soldados[i].peso;
        if (ratio > melhor_ratio)
        {
            melhor_ratio = ratio;
            atual = i;
        }
    }

    while (1)
    {
        // Marca o atual como visitado
        visitado[visitados++] = atual;

        // Recruta o máximo possível (mochila infinita)
        int max_qtd = (soldados[atual].peso == 0) ? 0 : (W - peso_total) / soldados[atual].peso;
        recrutados[atual] = max_qtd;
        habilidade_total += max_qtd * soldados[atual].habilidade;
        peso_total += max_qtd * soldados[atual].peso;

        // Encontra o melhor vizinho para ir (melhor eficiência, dentro do D restante)
        int proximo = -1;
        double melhor_score = -1;
        for (int i = 0; i < grafo[atual].qtd_vizinhos; i++)
        {
            int viz = grafo[atual].vizinhos[i].destino;
            int dist = grafo[atual].vizinhos[i].distancia;

            if (ja_foi(visitado, visitados, viz))
                continue;
            if (distancia_total + dist > D)
                continue;

            double ratio = (double)soldados[viz].habilidade / soldados[viz].peso;
            if (ratio > melhor_score)
            {
                melhor_score = ratio;
                proximo = viz;
            }
        }

        // Se não há para onde ir, parar
        if (proximo == -1)
            break;

        // Avança
        for (int i = 0; i < grafo[atual].qtd_vizinhos; i++)
        {
            if (grafo[atual].vizinhos[i].destino == proximo)
            {
                distancia_total += grafo[atual].vizinhos[i].distancia;
                break;
            }
        }
        atual = proximo;
    }

    // Escreve no arquivo de saída
    fprintf(out, "%d", habilidade_total);
    for (int i = 0; i < visitados; i++)
    {
        int id = visitado[i];
        fprintf(out, " %d %d", id + 1, recrutados[id]);
    }
    fprintf(out, "\n");

    free(visitado);
    free(recrutados);
}