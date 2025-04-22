#define BREADTH_FIRST_SEARCH_H

#include <stdio.h>
#include <stdbool.h>
#include "../game/game.h"

bool breadthFirstSearch(game* initial, int* finalMovesUsed);
void computeBreadthFirstSearch(game** games, char* filename);
