#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "../game/game.h"

void generateNoise(game* src, game* dest);
bool localBeamSearch(game* initial, int (*dist)(game*), int* finalMovesUsed);
void computeLocalBeamSearch(game** games, int (*dist)(game*), char* filename);