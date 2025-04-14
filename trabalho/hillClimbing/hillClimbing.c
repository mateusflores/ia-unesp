#include "hillClimbing.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../game/game.h"
#include "../exporter/exporter.h"
#include "../simulatedAnnealing/simulatedAnnealing.h"

int tmax = 10000;

int hillClimbing(game* initial, int (*dist)(game*)){
    game* current = newGame();
    copyGame(initial, current);
    int currentDist = dist(current);

    game* bestSolution = newGame();
    copyGame(initial, bestSolution);
    int bestSolutionDist = currentDist;

    game* newSolution = newGame();
    int newSolutionDist = 1000000;
    int movements = 0;

    for (int t = 0; t < tmax; t++) {
        copyGame(current, newSolution);

        int move = getRandomMove(newSolution);

        if (move != -1 && moveGame(newSolution, move)) {
            currentDist = dist(current);
            newSolutionDist = dist(newSolution);

            if (newSolutionDist < currentDist) {
                
                int distDiff = newSolutionDist - currentDist;

                if (acceptSolution(currentDist, distDiff)) {
                    copyGame(newSolution, current);
                    if (newSolutionDist < bestSolutionDist) {
                        copyGame(newSolution, bestSolution);
                        bestSolutionDist = newSolutionDist;
                    }
                    movements++;
                }
            }
        }

        if (endGame(current)) {
            delGame(current);
            delGame(newSolution);
            return true;
        }
    }

    return movements;
}

void computeHillClimbing(game** games, int (*dist)(game*), char* filename) {
    int moves[10];
    double times[10];
    
    for (int i = 0; i < 10; i++) {
        game* initial = games[i];
        
        clock_t begin = clock();
        int movesUsed = hillClimbing(initial, dist);
        clock_t end = clock();

        double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        moves[i] = movesUsed;
        times[i] = timeSpent;

        delGame(initial);
    }

    exportFile(filename, moves, times);
}