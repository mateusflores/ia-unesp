#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "particles.h"
#include "utils.h"

void initParticle(particle* p, int dim, double* x_0, double w){
/*
    Inicializa uma partícula.
    p: partícular a ser inicializada
    dim: dimensão do espaço de busca
    x_0: posição inicial
    w: inércia
*/
    //Posição inicial
    p->x = malloc(dim * sizeof(double));
    assert(p->x);
    copyArray(p->x, x_0, dim);
    //Posição ótima
    p->x_opt = malloc(dim * sizeof(double));
    assert(p->x_opt);
    copyArray(p->x_opt, x_0, dim);
    //Velocidade inicial é zero
    p->v = malloc(dim * sizeof(double));
    assert(p->v);
    for (int i = 0; i < dim; i++)
        p->v[i] = 0;
    //Dimensão
    p->dim = dim;
    //Inércia
    p->w = w;   
}

void freeParticle(particle* p){
    if (p!=NULL){
        if (p->x != NULL)
            free(p->x);
        if (p->x_opt != NULL)
            free(p->x_opt);
        if (p->v != NULL)
            free(p->v);
    }
}

void updateVelocity(particle* p, double* c1, double* c2, double* swarm_opt){
/*
    Função para atualizar a velocidade de uma partícula.
    p: partícula a ser atualizada
    c1 e c2: vetores de coeficientes
    swarm_opt: vetor ótimo do enxame
*/
    double* v_inertia = malloc(p->dim * sizeof(double));
    for (int i = 0; i < p->dim; i++) {
        v_inertia[i] = p->w * p->v[i];
    }

    //Termo individual da velocidade
    double* aux_sub1 = arraySub(p->x_opt, p->x, p->dim);
    double* aux_mul1 = arrayMul(c1, aux_sub1, p->dim);
    //Termo coletivo da velocidade
    double* aux_sub2 = arraySub(swarm_opt, p->x, p->dim);
    double* aux_mul2 = arrayMul(c2, aux_sub2, p->dim);


    double* sum = arraySum(v_inertia, aux_mul1, p->dim);
    double* new_velocity = arraySum(sum, aux_mul2, p->dim);

    //Atualiza a velocidade
    p->v = new_velocity;
    //Libera memória
    free(aux_sub1);
    free(aux_sub2);
    free(aux_mul1);
    free(aux_mul2);
    free(sum);
}

void updatePosition(particle* p, double (*cost)(double*)){
/*
    Função para atualiar a posição de uma parícula.
    p: partícula a ser atualizada
    cost: função custo
*/
    //Ponteiro para posição anterior
    double* aux_x = p->x;
    //Atualiza posição atual
    p->x = arraySum(aux_x, p->v, p->dim);
    //Checa se x_opt deve ser atualizada
    double cost_prev = cost(p->x_opt);
    double cost_new = cost(p->x);
    if (cost_prev>cost_new){
        copyArray(p->x_opt, p->x, p->dim);
    }
    //Libera memória
    free(aux_x);
}