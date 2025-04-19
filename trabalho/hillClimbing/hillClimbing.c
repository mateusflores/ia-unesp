#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "../simulatedAnnealing/simulatedAnnealing.h"
#include "../game/game.h"
#include "../exporter/exporter.h"

// Parameters
int maxIter = 1000000;

bool hillClimbing (game* initial, int (*dist)(game*), int* movesUsed) {
    game* current = newGame();
    game* newSolution = newGame();
    copyGame(initial, current);
    game* bestSolution = newGame();
    copyGame(initial, bestSolution);

    int bestEnergy = dist(bestSolution);

    (*movesUsed) = 0;

    for (int i = 0; i < maxIter; i++) {
        copyGame(current, newSolution);

        int move = getRandomMove(newSolution);
        if (move != -1 && moveGame(newSolution, move)) {
            int currentEnergy = dist(current);
            int newSolutionEnergy = dist(newSolution);
            int energyDiff = newSolutionEnergy - currentEnergy;

            if (acceptSolution(1000, energyDiff)) {
                copyGame(newSolution, current);
                if (newSolutionEnergy < bestEnergy) {
                    copyGame(newSolution, bestSolution);
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

    delGame(current);
    delGame(newSolution);
    return false;
}

void computeHillClimbing(game** games, int (*dist)(game*), char* filename) {
    int moves[10];
    double times[10];
    char victories[10];
    
    for (int i = 0; i < 10; i++) {
        int movesUsed = 0;
        game* initial = games[i];
        
        clock_t begin = clock();
        bool solved = hillClimbing(initial, dist, &movesUsed);
        clock_t end = clock();

        double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        moves[i] = movesUsed;
        times[i] = timeSpent;
        victories[i] = solved ? 'V' : 'D';

        delGame(initial);
    }

    exportFile(filename, moves, times, victories);
}