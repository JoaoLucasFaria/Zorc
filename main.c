#include <string.h>
#include <unistd.h>
#include "tp.h"
#include "dinamica.h"
#include "heuristica.h"

int main(int argc, char *argv[])
{
    struct rusage uso_inicio, uso_fim;
    char *entrada = NULL;
    char *saida = NULL;
    int estrategia = 0;
    int opcao;

    while ((opcao = getopt(argc, argv, "i:o:s:")) != -1)
    {
        switch (opcao)
        {
        case 'i':
            entrada = optarg;
            break;
        case 'o':
            estrategia = atoi(optarg);
            break;
        case 's':
            saida = optarg;
            break;
        default:
            fprintf(stderr, "Uso: %s -i <entrada.txt> -o <1 para Dinamica || 2 para heuristica> -s <saida.txt>\n", argv[0]);
            return 1;
        }
    }

    // Validação dos parâmetros obrigatórios
    if (!entrada || !saida || estrategia < 1 || estrategia > 2)
    {
        fprintf(stderr, "ERRO. Uso: ./tp2 -i <entrada.txt> -o <1 para Dinamica || 2 para heuristica> -s <saida.txt>\n"); 
        return 1;
    }

    FILE *fp = fopen(entrada, "r");
    if (!fp)
    {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE *out = fopen(saida, "w");
    if (!out)
    {
        perror("Erro ao criar arquivo de saída");
        fclose(fp);
        return 1;
    }

    // Leitura do número de instâncias
    int K;
    fscanf(fp, "%d", &K);

    getrusage(RUSAGE_SELF, &uso_inicio);

    // Processar cada instância
    for (int instancia = 0; instancia < K; instancia++)
    {
        int P, D, W, C;
        fscanf(fp, "%d %d %d %d", &P, &D, &W, &C);

        Soldado *soldados = le_soldados_arquivo(fp, P);
        Povo *grafo = cria_grafo_arquivo(fp, P, C);

        /*// DEBUG: imprimir os soldados
        fprintf(stderr, "\n=== SOLDADOS DA INSTÂNCIA %d ===\n", instancia + 1);
        for (int i = 0; i < P; i++)
        {
            fprintf(stderr, "Povo %d: peso = %d, habilidade = %d\n", i + 1, soldados[i].peso, soldados[i].habilidade);
        } // FIM DEBUG


        // DEBUG: imprimir o grafo
        fprintf(stderr, "\n=== GRAFO DA INSTÂNCIA %d ===\n", instancia + 1);
        for (int i = 0; i < P; i++)
        {
            fprintf(stderr, "Povo %d está conectado com: ", i + 1);
            for (int j = 0; j < grafo[i].qtd_vizinhos; j++)
            {
                int vizinho = grafo[i].vizinhos[j].destino + 1;
                int dist = grafo[i].vizinhos[j].distancia;
                fprintf(stderr, "%d(dist=%d) ", vizinho, dist);
            }
            fprintf(stderr, "\n");
        } // FIM DEBUG*/

        if (estrategia == 1)
        {
            // Estratégia ótima: busca exaustiva com programação dinâmica
            estrategia_otima(grafo, soldados, P, W, D, out);
        }
        else if (estrategia == 2)
        {
            // Estratégia heurística: aproximação gulosa baseada em eficiência
            estrategia_heuristica(grafo, soldados, P, W, D, out);
        }
        else
        {
            fprintf(stderr, "Estratégia %d inválida.\n", estrategia);
        }

        // Liberação de memória
        libera_soldados(soldados);
        libera_grafo(grafo, P);
    }

    fclose(fp);
    fclose(out);
    getrusage(RUSAGE_SELF, &uso_fim);
    imprimir_tempos(&uso_inicio, &uso_fim); // Imprime tempo de usuário e sistema
    return 0;
}