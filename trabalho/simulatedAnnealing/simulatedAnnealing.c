#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "simulatedAnnealing.h"
#include "../game/game.h"
#include "../exporter/exporter.h"

// Parameters
double initialTemp = 1000.0;
double finalTemp = 1e-6;
int maxIterations = 10000;

// Escolhe um número vizinho do zero aleatoriamente
int getRandomMove(game* G) {
    int zeroi, zeroj;
    int options[4], count = 0;

    // Localiza o zero
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (G->grid[i][j] == 0) {
                zeroi = i;
                zeroj = j;
            }

    // Verifica movimentos válidos (vizinhos ao zero)
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int d = 0; d < 4; d++) {
        int ni = zeroi + dirs[d][0];
        int nj = zeroj + dirs[d][1];
        if (ni >= 0 && ni < 3 && nj >= 0 && nj < 3) {
            options[count++] = G->grid[ni][nj];
        }
    }

    if (count == 0) return -1;
    return options[rand() % count];
}

bool acceptSolution(double temp, double energyDiff) {
    if (energyDiff < 0)
        return true;

    double r = ((double)rand() / RAND_MAX);
    if (r < exp(-energyDiff / temp))
        return true;

    return false;
}

bool simulatedAnnealing (game* initial, double coolingRate, int (*dist)(game*), game** bestSolution, int* movesUsed) {
    game* current = newGame();
    game* newSolution = newGame();
    copyGame(initial, current);
    copyGame(initial, *bestSolution);

    int bestEnergy = dist(*bestSolution);
    double currentTemp = initialTemp;

    while (currentTemp > finalTemp) {
        (*movesUsed) = 0;
        for (int i = 0; i < maxIterations; i++) {
            copyGame(current, newSolution);

            int move = getRandomMove(newSolution);
            if (move != -1 && moveGame(newSolution, move)) {
                int currentEnergy = dist(current);
                int newSolutionEnergy = dist(newSolution);
                int energyDiff = newSolutionEnergy - currentEnergy;

                if (acceptSolution(currentTemp, energyDiff)) {
                    copyGame(newSolution, current);
                    if (newSolutionEnergy < bestEnergy) {
                        copyGame(newSolution, *bestSolution);
                        bestEnergy = newSolutionEnergy;
                    }
                    (*movesUsed)++;
                }
            }

            if (endGame(current)) {
                delGame(current);
                delGame(newSolution);
                return true;
            }
        }

        currentTemp *= coolingRate;
    }

    delGame(current);
    delGame(newSolution);
    return false;
}

void computeSimulatedAnnealing(game** games, double* betas, int betasQuant, int (*dist)(game*), char* filename) {
    int moves[10];
    double times[10];
    char victories[10];
    
    for (int i = 0; i < 10; i++) {
        game* initial = games[i];
        game *bestAttempt = newGame();
        bestAttempt->grid = malloc(3 * sizeof(int *));

        for (int i = 0; i < 3; i++)
            bestAttempt->grid[i] = malloc(3 * sizeof(int));

        bool foundAnySolution = false;
        int bestMovesUsed = 1000000;
        int bestPartialCost = 1000000;

        double bestTime = 0.0;
        bool solved = false;

        for (int i = 0; i < betasQuant; i++) {
            int currentMoves = 0;

            game *temp = newGame();
            temp->grid = malloc(3 * sizeof(int *));
            for (int i = 0; i < 3; i++)
                temp->grid[i] = malloc(3 * sizeof(int));

            clock_t begin = clock();
            solved = simulatedAnnealing(initial, betas[i], dist, &temp, &currentMoves);
            clock_t end = clock();
            double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

            if (solved) {
                if (!foundAnySolution || currentMoves < bestMovesUsed) {
                    copyGame(temp, bestAttempt);
                    bestMovesUsed = currentMoves;
                    bestTime = timeSpent;
                    foundAnySolution = true;
                }
            }
            else {
                int currentCost = dist(temp);
                if (!foundAnySolution && currentCost < bestPartialCost) {
                    copyGame(temp, bestAttempt);
                    bestPartialCost = currentCost;
                    bestMovesUsed = currentMoves;
                    bestTime = timeSpent;
                }
            }

            delGame(temp);
        }
        moves[i] = bestMovesUsed;
        times[i] = bestTime;
        victories[i] = solved ? 'V' : 'D';

        delGame(initial);
        delGame(bestAttempt);
    }

    exportFile(filename, moves, times, victories);
}