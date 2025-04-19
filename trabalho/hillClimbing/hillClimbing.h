#include <stdio.h>
#include "../game/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "../simulatedAnnealing/simulatedAnnealing.h"
#include "../game/game.h"
#include "../exporter/exporter.h"

bool hillClimbing (game* initial, int (*dist)(game*), int* movesUsed);
void computeHillClimbing(game** games, int (*dist)(game*), char* filename);