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

    if(startRow == 0 && startCol == 0){
        return E.grid[0][0];
    }else{
        place invalidPlace = {-1, -1, false};
        return invalidPlace;
    }
    
}

int getDistanceBetween(place P1, place P2) {
    int yDistance = abs(P1.col - P2.col);
    int xDistance = abs(P1.row - P2.row);
    return xDistance + yDistance;
}

bool hasEnoughBattery(cleaner* C, place P) {
    int cleanAction = P.dirt ? 1 : 0;
    int distCleanerToPlace = getDistanceBetween(*C->whereCleaner, P);
    int distPlaceToCharger = getDistanceBetween(P, *C->whereCharger);
    if (C->battery - distCleanerToPlace - distPlaceToCharger - cleanAction  >= 0) {
        return true;
    }
    return false;
}

bool checkIfDirtExists(int dirtCol, int dirtRow) {
    if (dirtCol == -1 && dirtRow == -1) {
        printf("Nenhuma sujeira encontrada no ambiente\n");        
        return false;
    }
    return true;
}

void cleanEnviroment(cleaner* C, enviroment E){
/*
    Plano de limpeza para um agente que conhece o ambiente e localização de sujeiras.
*/
    while (true) {
        place dirtPlace = diamondSearch(E, C->whereCleaner->row, C->whereCleaner->col);

        if (!checkIfDirtExists(dirtPlace.col, dirtPlace.row)) {
            printf("Retornando ao carregador...");
            charge(C, E);
            break;
        }

        if (!hasEnoughBattery(C, dirtPlace)) {
            //Retorna para carregar bateria
            charge(C,E);
            
            place newDirtPlace = diamondSearch(E, C->whereCleaner->row, C->whereCleaner->col);

            if (!checkIfDirtExists(dirtPlace.col, dirtPlace.row)) {
                printf("Retornando ao carregador...");
                charge(C, E);
                break;
            }

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


