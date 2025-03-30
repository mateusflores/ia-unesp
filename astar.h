#include "game.h"
#include "tree.h"

typedef struct {
    node nd;
    int f;
} AStarNode;

int hfora(game* G);
int hM(game* G);
int compare(const void* a, const void* b);
node AStarSearch(game* G, int (*heuristic)(game*));