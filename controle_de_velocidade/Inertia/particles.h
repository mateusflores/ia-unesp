
#ifndef PARTICLES_H
#define PARTICLES_H

typedef struct {
    double* x; //posição atual
    double* x_opt; //posição ótima
    double* v; //velocidade
    int dim; //dimensão
    double w; //inércia
} particle;

void initParticle(particle* p, int dim, double* x_0, double w);
void freeParticle(particle* p);
void updateVelocity(particle* p, double* c1, double* c2, double* swarm_opt);
void updatePosition(particle* p, double (*cost)(double*));

#endif