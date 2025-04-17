#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DIMENSION 5         // Tamanho da solução
#define NUMBER_OF_BEAMS 10                // Número de feixes
#define MAX_ITER 1000      // Iterações principais
#define MAX_LOCAL 1000      // Iterações da busca local
#define LIMIT 100         // Limite dos valores possíveis para solução

#define INITIAL_TEMP 1000.0
#define FINAL_TEMP 0.001
#define COOLING_RATE 0.9

// Função objetivo (exemplo): minimizar soma dos quadrados
double funcX2(double *sol) {
    double soma = 0;
    for (int i = 0; i < DIMENSION; i++)
        soma += sol[i] * sol[i];
    return soma;
}

// Gera solução aleatória
void getNewSolution(double *sol) {
    for (int i = 0; i < DIMENSION; i++) {
        sol[i] = ((double)rand() / RAND_MAX) * (2.0 * LIMIT) - LIMIT;  // Valores entre -LIMITE e +LIMITE
    }
}

// Cria vizinho estocástico
void generateNoise(double *src, double *dest) {
    for (int i = 0; i < DIMENSION; i++) {
        double noise = ((double)rand() / RAND_MAX) * 2.0 - 1.0;  // [-1.0, +1.0]
        dest[i] = src[i] + noise;
    }
}

void simulated_annealing(double *initial, double *bestSolution) {
    double current[DIMENSION], neighbor[DIMENSION];
    double currentTemp = INITIAL_TEMP;
    double currentEnergy, neighborEnergy, energyDiff;

    // Copiar solução inicial
    for (int i = 0; i < DIMENSION; i++)
        current[i] = initial[i];
    currentEnergy = funcX2(current);

    while (currentTemp > FINAL_TEMP) {
        for (int i = 0; i < 10; i++) {  // Vizinhos por temperatura
            generateNoise(current, neighbor);
            neighborEnergy = funcX2(neighbor);
            energyDiff = neighborEnergy - currentEnergy;

            if (energyDiff < 0 || ((double) rand() / RAND_MAX) < exp(-energyDiff / currentTemp)) {
                for (int j = 0; j < DIMENSION; j++)
                    current[j] = neighbor[j];
                currentEnergy = neighborEnergy;
            }
        }
        currentTemp *= COOLING_RATE;
    }

    for (int i = 0; i < DIMENSION; i++)
        bestSolution[i] = current[i];
}

int main() {
    srand(time(NULL));

    double beams[NUMBER_OF_BEAMS][DIMENSION];
    double newbeams[NUMBER_OF_BEAMS][DIMENSION];
    double bestSolution[DIMENSION];
    double dist;
    double bestDist = 100000000;

    // Inicialização
    for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
        getNewSolution(beams[i]);
        simulated_annealing(beams[i], beams[i]);
    }

    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Seleciona melhor solução entre os feixes
        int idx_melhor = 0;
        bestDist = funcX2(beams[0]);
        for (int i = 1; i < NUMBER_OF_BEAMS; i++) {
            dist = funcX2(beams[i]);
            if (dist < bestDist) {
                bestDist = dist;
                idx_melhor = i;
            }
        }

        for (int i = 0; i < DIMENSION; i++)
            bestSolution[i] = beams[idx_melhor][i];

        // Gera nova população baseada no melhor
        for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
            generateNoise(bestSolution, newbeams[i]);
            simulated_annealing(newbeams[i], beams[i]);
        }

        printf("Iter %d: Melhor avaliacao = %.2f\n", iter, bestDist);
    }

    printf("Melhor solucao encontrada: ");
    for (int i = 0; i < DIMENSION; i++) 
        printf("%.2f ", bestSolution[i]);
    printf("\nAvaliacao final: %.2f\n", bestDist);
    printf("Solucao correta: 0.00");

    return 0;
}