#include <stdio.h>
#include "../game/game.h"

int hillClimbing(game* initial, int (*dist)(game*));
void computeHillClimbing(game** games, int (*dist)(game*), char* filename);