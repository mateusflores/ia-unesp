#include "../game/game.h"
#include <math.h>
#include <stdlib.h>

int manhattan(game* G) {
    int dist = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int val = G->grid[i][j];
            if (val == 0) 
                continue;
            int target_i = val / 3;
            int target_j = val % 3;
            dist += abs(i - target_i) + abs(j - target_j);
        }
    return dist;
}