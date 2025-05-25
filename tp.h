#ifndef TP_H
#define TP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/resource.h>

typedef struct {
    int destino;
    int distancia;
} Aresta;

typedef struct {
    Aresta *vizinhos;
    int qtd_vizinhos;
} Povo;

typedef struct {
    int peso;
    int habilidade;
} Soldado;

Soldado *le_soldados_arquivo(FILE *fp, int P);
Povo *cria_grafo_arquivo(FILE *fp, int P, int C);
void libera_soldados(Soldado *soldados);
void libera_grafo(Povo *grafo, int P);
void imprimir_tempos(struct rusage *inicio, struct rusage *fim);

#endif