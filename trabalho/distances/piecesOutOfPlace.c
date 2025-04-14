#include "../game/game.h"
#include <math.h>
#include <stdlib.h>

int piecesOutOfPlace(game* G) {
    int outOfPlace = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int val = G->grid[i][j];
            if (val != 0 && val != i * 3 + j) {
                outOfPlace++;
            }
        }
    }
    return outOfPlace;
}