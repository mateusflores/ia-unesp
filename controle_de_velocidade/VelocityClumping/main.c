#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "swarm.h"

#define PI 3.141592684

double rastrigin(double* x){
    double out = 2;
    out += x[0]*x[0]-cos(2*PI*x[0]);
    out += x[1]*x[1]-cos(2*PI*x[1]);
    return out;
}

void writeArray(char* fname, double* out, int n){
    FILE* fp = fopen(fname,"w");
    for (int i=0; i<n; i++){
        fprintf(fp,"%.17g \n",out[i]);
    }
    fclose(fp);
}

int main(){

    //Ótimo individual é priorizado no cálculo da velocidade
    double c1[2] = {0.05,0.05};
    double c2[2] = {0.01,0.01};
/*
    //Ótimo do enxame é priorizado no cálculo da velocidade
    double c1[2] = {0.01,0.01};
    double c2[2] = {0.05,0.05};
*/
    double v_max = 0.1;

    swarm S; 
    initSwarm(&S, 5, 2, c1, c2, v_max);

    int tmax = 1000;
    double out[tmax];
    for (int t = 0; t < tmax; t++){
        updateSwarm(&S, rastrigin);
        out[t] = rastrigin(S.x_opt);
    }

    writeArray("resultados_N=5.dat",out,tmax);
    return 0;
}