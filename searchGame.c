#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include "astar.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
    int type = 0;

    game* G = newGame();
    G->grid[0][0] = 0;
    G->grid[0][1] = 3;
    G->grid[0][2] = 8;
    G->grid[1][0] = 1;
    G->grid[1][1] = 2;
    G->grid[1][2] = 6;
    G->grid[2][0] = 7;
    G->grid[2][1] = 4;
    G->grid[2][2] = 5;

    printf("Escolha o modo de busca:\n  1 - H Fora\n  2 - H Manhattan\n");
    scanf("%d", &type);

    node nd;

    if (type == 1) {
        nd = AStarSearch(G, hfora);
    }
    else if (type == 2) {
        nd = AStarSearch(G, hM);
    }
    else {
        printf("Opcao invalida. Tente novamente.");
        exit(1);
    }

    node aux = nd;
    while (aux!=NULL){
        printGame(aux->state);
        printf("\n");
        aux = aux->parent;
    }
    
    return 0;
}