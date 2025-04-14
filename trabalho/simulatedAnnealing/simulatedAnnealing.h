#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "../game/game.h"

int getRandomMove(game* G);
bool simulatedAnnealing (game* initial, double coolingRate, int (*dist)(game*), game** bestSolution, int* movesUsed);
bool acceptSolution(double temp, double energyDiff);
void computeSimulatedAnnealing(game** games, double* betas, int betasQuant, int (*dist)(game*), char* filename);