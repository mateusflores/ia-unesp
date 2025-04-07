#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include "localBeamSearch.h"

// Parameters
int beamWidth = 3;
int neighborsPerSolution = 5;
int maxIterations = 1000;
double threshold = 1e-6;

double getNewSolution(double current, double range) {
    return current + ((double)rand() / RAND_MAX * 2 - 1) * range;
}

int compareCandidates(const void *a, const void *b) {
    Candidate *candidateA = (Candidate *)a;
    Candidate *candidateB = (Candidate *)b;
    if (candidateA->value < candidateB->value) 
        return -1;
    if (candidateA->value > candidateB->value) 
        return 1;
    return 0;
}

double localBeamSearch(double (*func)(double)) {
    srand(time(NULL));
    
    // Inicializa o feixe com soluções aleatórias
    Candidate beam[beamWidth];
    for (int i = 0; i < beamWidth; i++) {
        beam[i].solution = ((double)rand() / RAND_MAX * 10) - 5;
        beam[i].value = func(beam[i].solution);
    }
    
    // Ordena o feixe inicial
    qsort(beam, beamWidth, sizeof(Candidate), compareCandidates);
    
    for (int i = 0; i < maxIterations; i++) {
        Candidate candidates[beamWidth * neighborsPerSolution];
        int candidateCount = 0;
        
        // Para cada solução no feixe, gera vizinhos
        for (int i = 0; i < beamWidth; i++) {
            for (int j = 0; j < neighborsPerSolution; j++) {
                double newSolution = getNewSolution(beam[i].solution, 1.0);
                candidates[candidateCount].solution = newSolution;
                candidates[candidateCount].value = func(newSolution);
                candidateCount++;
            }
        }
        
        // Ordena todos os candidatos (vizinhos + feixe atual)
        qsort(candidates, candidateCount, sizeof(Candidate), compareCandidates);
        
        // Seleciona os beamWidth melhores para o próximo feixe
        for (int i = 0; i < beamWidth; i++) {
            beam[i] = candidates[i];
        }
        
        // Critério de parada opcional: se não houve melhoria significativa
        double solutionDiff = beam[0].value - beam[beamWidth - 1].value;
        if (solutionDiff < 0) {
            solutionDiff *= -1;
        }
        if (i > 0 && solutionDiff < threshold) {
            break;
        }
    }
    
    return beam[0].solution;
}