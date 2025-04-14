#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game/game.h"
#include "simulatedAnnealing/simulatedAnnealing.h"
#include "hillClimbing/hillClimbing.h"
#include "distances/manhattam.h"
#include "distances/piecesOutOfPlace.h"
#include "game/generateGames.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Error: Not enough arguments provided.\n");
        return 1;
    }

    char algorithmType[4]; // 3 chars + 1 for null terminator
    char distanceAlgorithm[4];     // 3 chars + 1 for null terminator

    snprintf(algorithmType, sizeof(algorithmType), "%s", argv[1]);
    snprintf(distanceAlgorithm, sizeof(distanceAlgorithm), "%s", argv[2]);

    if (strcmp(algorithmType, "sa") == 0) {
        printf("Algoritmo: Simulated Annealing\n");

        double betas[] = {0.85, 0.90, 0.95, 0.98, 0.995};
        int betasQuant = sizeof(betas) / sizeof(betas[0]);

        if (strcmp(distanceAlgorithm, "mht") == 0) {
            computeSimulatedAnnealing(generateGames(), betas, betasQuant, manhattan, "sa_mht.dat");
        } 
        else if (strcmp(distanceAlgorithm, "pop") == 0) {
            computeSimulatedAnnealing(generateGames(), betas, betasQuant, piecesOutOfPlace, "sa_pop.dat");
        } 
        else {
            fprintf(stderr, "Error: Invalid distance algorithm. Use 'mht' or 'pop'.\n");
            return 1;
        }
    } else if (strcmp(algorithmType, "lbs") == 0) {
        printf("Algoritmo: Local Beam Search\n");
        if (strcmp(distanceAlgorithm, "mht") == 0) {
            printf("Distância: Manhattan\n");
        } else if (strcmp(distanceAlgorithm, "pop") == 0) {
            printf("Distância: Pieces outside of place\n");
        } else {
            fprintf(stderr, "Error: Invalid distance algorithm. Use 'mht' or 'pop'.\n");
            return 1;
        }
    } else if (strcmp(algorithmType, "hc") == 0) {
        printf("Algoritmo: Hill Climbing\n");
        if (strcmp(distanceAlgorithm, "mht") == 0) {
            computeHillClimbing(generateGames(), manhattan, "hc_mht.dat");
        } else if (strcmp(distanceAlgorithm, "pop") == 0) {
            computeHillClimbing(generateGames(), piecesOutOfPlace, "hc_pop.dat");
        } else {
            fprintf(stderr, "Error: Invalid distance algorithm. Use 'mht' or 'pop'.\n");
            return 1;
        }
    } else if (strcmp(algorithmType, "as") == 0) {
        printf("Algoritmo: A-star\n");
        if (strcmp(distanceAlgorithm, "mht") == 0) {
            printf("Distância: Manhattan\n");
        } else if (strcmp(distanceAlgorithm, "pop") == 0) {
            printf("Distância: Pieces outside of place\n");
        } else {
            fprintf(stderr, "Error: Invalid distance algorithm. Use 'mht' or 'pop'.\n");
            return 1;
        }
    } else if (strcmp(algorithmType, "bfs") == 0) {
        printf("Algoritmo: Breadth-first search\n");
        if (strcmp(distanceAlgorithm, "mht") == 0) {
            printf("Distância: Manhattan\n");
        } else if (strcmp(distanceAlgorithm, "pop") == 0) {
            printf("Distância: Pieces outside of place\n");
        } else {
            fprintf(stderr, "Error: Invalid distance algorithm. Use 'mht' or 'pop'.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error: Invalid algorithm type. Use 'sa', 'lbs', 'hc', 'as', or 'bfs'.\n");
        return 1;
    }
}