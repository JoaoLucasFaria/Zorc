#include <stdio.h>
#include <stdlib.h>
#include "tp.h"

//Libera a memória alocada para o grafo
void libera_grafo(Povo* grafo, int P) {
    for (int i = 0; i < P; i++) {
        free(grafo[i].vizinhos);
    }
    free(grafo);
}

//Libera a memória alocada para os soldados
void libera_soldados(Soldado* soldados) {
    free(soldados);
}

//Lê os dados dos soldados de cada povo a partir do arquivo de entrada
Soldado* le_soldados_arquivo(FILE *fp, int P) {
    Soldado* soldados = malloc(P * sizeof(Soldado));
    for (int i = 0; i < P; i++) {
        int id, peso, habilidade;
        fscanf(fp, "%d %d %d", &id, &peso, &habilidade);
        if (habilidade<=0) habilidade=1;
        if (habilidade>=11) habilidade = 10;
        soldados[id - 1].peso = peso;
        soldados[id - 1].habilidade = habilidade;
    }
    return soldados;
}

//Cria a estrutura do grafo a partir das conexões informadas no arquivo de entrada
Povo* cria_grafo_arquivo(FILE *fp, int P, int C) {
    Povo* grafo = malloc(P * sizeof(Povo));
    for (int i = 0; i < P; i++) {
        grafo[i].qtd_vizinhos = 0;
        grafo[i].vizinhos = NULL;
    }

    for (int i = 0; i < C; i++) {
        int a, b, d;
        fscanf(fp, "%d %d %d", &a, &b, &d);
        a--; b--;

        grafo[a].vizinhos = realloc(grafo[a].vizinhos, (grafo[a].qtd_vizinhos + 1) * sizeof(Aresta));
        grafo[a].vizinhos[grafo[a].qtd_vizinhos++] = (Aresta){b, d};

        grafo[b].vizinhos = realloc(grafo[b].vizinhos, (grafo[b].qtd_vizinhos + 1) * sizeof(Aresta));
        grafo[b].vizinhos[grafo[b].qtd_vizinhos++] = (Aresta){a, d};
    }

    return grafo;
}

//imprime os tempos de usuário e de sistema no terminal
void imprimir_tempos(struct rusage *inicio, struct rusage *fim) {
    long segundos_usuario = fim->ru_utime.tv_sec - inicio->ru_utime.tv_sec;
    long microssegundos_usuario = fim->ru_utime.tv_usec - inicio->ru_utime.tv_usec;

    long segundos_sistema = fim->ru_stime.tv_sec - inicio->ru_stime.tv_sec;
    long microssegundos_sistema = fim->ru_stime.tv_usec - inicio->ru_stime.tv_usec;

    double tempo_usuario = segundos_usuario + microssegundos_usuario / 1e6;
    double tempo_sistema = segundos_sistema + microssegundos_sistema / 1e6;

    printf("Tempo de usuário: %.6f segundos\n", tempo_usuario);
    printf("Tempo de sistema: %.6f segundos\n", tempo_sistema);
}