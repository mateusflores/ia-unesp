#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "../game/game.h"
#include "../exporter/exporter.h"
#include "../simulatedAnnealing/simulatedAnnealing.h"

#define NUMBER_OF_BEAMS 5                // Número de feixes
#define MAX_ITER 1000      // Iterações principais
#define MAX_LOCAL 1000      // Iterações da busca local
#define LIMIT 100         // Limite dos valores possíveis para solução

int localBeamSearchCoolingRate = 0.95;

// Cria vizinho estocástico
void generateNoise(game* src, game* dest) {
    copyGame(src, dest);
    int tries = 0;
    while (tries < 10) {
        int move = getRandomMove(dest);
        if (move != -1 && moveGame(dest, move))
            break;
        tries++;
    }
}

bool localBeamSearch(game* initial, int (*dist)(game*), int* finalMovesUsed) {
    srand(time(NULL));
    
    game* beams[NUMBER_OF_BEAMS];
    game* newBeams[NUMBER_OF_BEAMS];
    game* bestSolution = newGame();
    copyGame(initial, bestSolution);

    int movesUsed[NUMBER_OF_BEAMS];
    int localMovesUsed = 0;
    (*finalMovesUsed) = 0;

    // Inicializar feixes com jogos aleatórios e aplicar SA
    for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
        beams[i] = newGame();
        copyGame(initial, beams[i]);
        movesUsed[i] = 0;
        game* localBest = newGame();
        bool solved = simulatedAnnealing(beams[i], localBeamSearchCoolingRate, dist, &localBest, &movesUsed[i]);
        delGame(beams[i]);
        beams[i] = localBest;
        if (solved) {
            copyGame(beams[i], bestSolution);
            (*finalMovesUsed) += movesUsed[i];
            for (int j = 0; j <= i; j++) {
                if (j != i) {
                    delGame(beams[j]);
                }
            }
            return true;
        }
    }

    for (int i = 0; i < MAX_ITER; i++) {
        // Encontrar o melhor feixe
        int bestIdx = 0;
        int bestScore = dist(beams[0]);
        for (int i = 1; i < NUMBER_OF_BEAMS; i++) {
            int score = dist(beams[i]);
            if (score < bestScore) {
                bestScore = score;
                bestIdx = i;
            }
        }

        copyGame(beams[bestIdx], bestSolution);
        (*finalMovesUsed) += movesUsed[bestIdx];
        if (bestScore == 0) {
            for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
                delGame(beams[i]);
            }
            return true;
        }

        // Gera novos feixes perturbando o melhor
        for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
            game* newSolution = newGame();
            generateNoise(bestSolution, newSolution);
            movesUsed[i] = 0;
            game* localBest = newGame();
            bool solved = simulatedAnnealing(newSolution, localBeamSearchCoolingRate, dist, &localBest, &movesUsed[i]);
            delGame(newSolution);
            delGame(beams[i]);
            beams[i] = localBest;
            if (solved) {
                copyGame(beams[i], bestSolution);
                (*finalMovesUsed) += movesUsed[i];
                // libera memória dos demais
                for (int j = 0; j < NUMBER_OF_BEAMS; j++) {
                    if (j != i) {
                        delGame(beams[j]);
                    }
                }
                return true;
            }
        }
    }

    int bestIdx = 0;
    int bestScore = dist(beams[0]);
    for (int i = 1; i < NUMBER_OF_BEAMS; i++) {
        int score = dist(beams[i]);
        if (score < bestScore) {
            bestScore = score;
            bestIdx = i;
        }
    }
    copyGame(beams[bestIdx], bestSolution);
    (*finalMovesUsed) += movesUsed[bestIdx];

    for (int i = 0; i < NUMBER_OF_BEAMS; i++) {
        delGame(beams[i]);
    }
    return false;
}

void computeLocalBeamSearch(game** games, int (*dist)(game*), char* filename) {
    int moves[10];
    double times[10];
    char victories[10];
    
    for (int i = 0; i < 10; i++) {
        int movesUsed = 0;
        game* initial = games[i];
        
        clock_t begin = clock();
        bool solved = localBeamSearch(initial, dist, &movesUsed);
        clock_t end = clock();

        double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        moves[i] = movesUsed;
        times[i] = timeSpent;
        victories[i] = solved ? 'V' : 'D';

        delGame(initial);
    }

    exportFile(filename, moves, times, victories);
}