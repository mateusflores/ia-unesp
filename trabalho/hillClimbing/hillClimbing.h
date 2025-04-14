#include <stdio.h>
#include "../game/game.h"

bool hillClimbing(game* initial, int (*dist)(game*), int* movesUsed);
void computeHillClimbing(game** games, int (*dist)(game*), char* filename);