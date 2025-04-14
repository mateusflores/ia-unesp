
#include <stdlib.h>
#include <stdio.h>
#include "particles.h"

#ifndef SWARM_H
#define SWARM_H

typedef struct{
    int N; //número de partículas
    particle* P; //conjunto de partículas
    int dim; //dimensão
    double* x_opt; //posição ótima
    //Vetores-coeficientes
    double* c1;
    double* c2;
    double v_max; //velocidade máxima
} swarm;

void initSwarm(swarm* S, int N, int dim, double* c1, double* c2, double v_max);
void freeSwarm(swarm* S);
void updateSwarm(swarm* S, double (*cost)(double*));

#endif