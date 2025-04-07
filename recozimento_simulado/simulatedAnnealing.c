#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "simulatedAnnealing.h"

// Parameters
double initialTemp = 1000.0;
double finalTemp = 0.1;
double coolingRate = 0.95;
int maxIterations = 100;

double getNewSolution(double current, double range) {
    return current + ((double)rand() / RAND_MAX * 2 - 1) * range;
}

bool acceptSolution(double temp, double energyDiff) {
    if (energyDiff < 0)
        return true;

    double r = ((double)rand() / RAND_MAX);
    if (r < exp(-energyDiff / temp))
        return true;

    return false;
}

double simulatedAnnealing (double (*func)(double)) {
    srand(time(NULL));
    double currentSolution = ((double)rand() / RAND_MAX) * 10 - 5;
    double currentEnergy = func(currentSolution);
    double bestSolution = currentSolution;
    double bestEnergy = currentEnergy;
    
    double currentTemp = initialTemp;
    
    while (currentTemp > finalTemp) {
        for (int i = 0; i < maxIterations; i++) {
            double newSolution = getNewSolution(currentSolution, 1.0);
            double newSolutionEnergy = func(newSolution);
            
            double energyDiff = newSolution - newSolutionEnergy;
            
            // Decide se aceita o novo estado
            if (acceptSolution(currentTemp, energyDiff)) {
                currentSolution = newSolution;
                currentEnergy = newSolutionEnergy;
                
                // Atualiza a melhor solução encontrada
                if (currentEnergy < bestEnergy) {
                    bestSolution = currentSolution;
                    bestEnergy = currentEnergy;
                }
            }
        }
        
        // Resfria o sistema
        currentTemp *= coolingRate;
    }
    
    return bestSolution;
}