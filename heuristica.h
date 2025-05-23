#ifndef HEURISTICA_H
#define HEURISTICA_H

#include "tp.h"

// Estratégia heurística gulosa (baseada em habilidade/peso)
void estrategia_heuristica(Povo *grafo, Soldado *soldados, int P, int W, int D, FILE *out);

#endif