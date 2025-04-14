
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

game* newGame(){
/*
    Inicialização de um jogo.
*/
    game* G = malloc(sizeof(game));
    G->grid = malloc(3*sizeof(int*));
    G->grid[0] = malloc(3*sizeof(int));
    G->grid[1] = malloc(3*sizeof(int));
    G->grid[2] = malloc(3*sizeof(int));
    return G;
}

void delGame(game* G){
    free(G->grid[0]);
    free(G->grid[1]);
    free(G->grid[2]);
    free(G->grid);
}

void initGame(game* G){
    //Inicializa grid com zeros
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            G->grid[i][j] = 0;
    //Inicializa gerador de números aleatórios
    srand(time(NULL));
    //Inclui números de 1 a 8 aleatoriamente
    for (int k=1; k<9; k++){
        int i = rand()%3;
        int j = rand()%3;
        while (G->grid[i][j]!=k){
            if (G->grid[i][j]==0)
                G->grid[i][j] = k;
            else{
                i = rand()%3;
                j = rand()%3;
            }
        }
    }
}

bool endGame(game* G){
/*
    Checa se o jogo foi vencido.
*/
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (G->grid[i][j]!=i*3+j)
                return false;
        }
    printf("Voce venceu o jogo! \n");
    return true;
}

void printGame(game* G){
/*
    Imprime estado atual do jogo na tela
*/
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            printf("%d  ",G->grid[i][j]);
        }
        printf("\n");
    }
}

void copyGame(game* G, game* target){
    //Copia G para target
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            target->grid[i][j] = G->grid[i][j];
        }
}

bool moveGame(game* G, int num){
/*
    Troca num e zero se estas forem vizinhas no grid.
*/
    int zeroi, zeroj;
    int numi, numj;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (G->grid[i][j]==0){
                zeroi = i; zeroj = j;
            }
            if (G->grid[i][j]==num){
                numi = i; numj = j;
            }
        }
    
    if (numi==zeroi || numj==zeroj){
        if (numi==zeroi+1 || numi==zeroi-1){
            G->grid[numi][numj] = 0;
            G->grid[zeroi][zeroj] = num;
            return true;
        }
        if (numj==zeroj+1 || numj==zeroj-1){
            G->grid[numi][numj] = 0;
            G->grid[zeroi][zeroj] = num;
            return true;
        }
    }
    return false;
}

bool applyMove(game* state, int direction) {
    // Find blank position
    int blank_row, blank_col;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state->grid[i][j] == 0) {
                blank_row = i;
                blank_col = j;
            }
        }
    }

    // Calculate target position based on direction
    int target_row = blank_row, target_col = blank_col;
    switch(direction) {
        case 0: target_row--; break; // Up
        case 1: target_row++; break; // Down
        case 2: target_col--; break; // Left
        case 3: target_col++; break; // Right
        default: return false;
    }

    // Validate move
    if(target_row < 0 || target_row >= 3 || 
       target_col < 0 || target_col >= 3) {
        return false;
    }

    // Swap tiles
    state->grid[blank_row][blank_col] = state->grid[target_row][target_col];
    state->grid[target_row][target_col] = 0;
    return true;
}

bool equal(game* A, game* B){
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (A->grid[i][j]!=B->grid[i][j]){
                return false;
            }
        }
    return true;
}