#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    node nd;
    int f; 
} AStarNode;

int hfora(game* G) {
    int dist = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (G->grid[i][j] != i * 3 + j) {
                dist++;
            }
        }
    }
    return dist;
}

int hM(game* G) {
    int dist = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value = G->grid[i][j];
            if (value != 0) {
                int targetX = (value - 1) / 3;
                int targetY = (value - 1) % 3;
                dist += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return dist;
}

int compare(const void* a, const void* b) {
    return ((AStarNode*)a)->f - ((AStarNode*)b)->f;
}

node AStarSearch(game* G, int (*heuristic)(game*)) {
    node nd = childNode(NULL, 0);
    copyGame(G, nd->state);
    nd->h = heuristic(nd->state);
    if (endGame(G)) {
        return nd;
    }

    fila frontier = newFila();
    insert(frontier, nd);
    fila explored = newFila();
    insert(explored, nd);

    while (frontier->first != NULL) {
        node parent = pop(frontier);

        game* aux = newGame();
        copyGame(parent->state, aux);

        for (int k = 1; k < 9; k++) {
            if (moveGame(aux, k)) {
                node child = childNode(parent, k);
                child->h = heuristic(child->state);

                if (!isIn(explored, child)) {
                    if (endGame(child->state)) {
                        return child;
                    }

                    insert(frontier, child);
                    insert(explored, child);
                } else {
                    delNode(child);
                }

                moveGame(aux, k);
            }
        }

        delGame(aux);
    }

    return NULL;
}