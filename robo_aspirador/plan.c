#include "plan.h"
#include "stdio.h"
#include <unistd.h>

place diamondSearch(enviroment E, int startRow, int startCol) {
    int maxDist = (E.h > E.w) ? E.h : E.w;

    for (int dist = 0; dist < maxDist; dist++) {
        for (int i = -dist; i <= dist; i++) {
            for (int j = -dist; j <= dist; j++) {
                if (abs(i) + abs(j) == dist) {
                    int row = startRow + i;
                    int col = startCol + j;

                    if (row >= 0 && row < E.h && col >= 0 && col < E.w && E.grid[row][col].dirt) {
                        return E.grid[row][col];
                    }
                }
            }
        }
    }

    place invalidPlace = {-1, -1, false};
    return invalidPlace;
}



void cleanEnviroment(cleaner* C, enviroment E){
/*
    Plano de limpeza para um agente que conhece o ambiente, mas não sabe onde 
    está a sujeira a ser limpada.
*/
    while (true) {
        place dirtPlace = diamondSearch(E, C->whereCleaner->row, C->whereCleaner->col);
        if (dirtPlace.col == -1 && dirtPlace.row == -1) {
            printf("Nenhuma sujeira encontrada no ambiente\n");
            break;
        }

        if (C->battery < E.h + E.w) {
            //Retorna para carregar bateria
            charge(C,E);
            
            place newDirtPlace = diamondSearch(E, C->whereCleaner->row, C->whereCleaner->col);

            int p = newDirtPlace.row;
            int t = newDirtPlace.col;

            //Retorna para posição da nova sujeira encontrada
            goTarget(C,E,&E.grid[p][t]);
        }
        else {
            goTarget(C,E,&E.grid[dirtPlace.row][dirtPlace.col]);
        }
        clean(C);
    }
}


