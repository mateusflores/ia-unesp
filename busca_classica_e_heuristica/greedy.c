#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows
#include "game.h"
#include "tree.h"
#include "bfstree.h"
#include "greedy.h"

int winDist(game* G){
/*
    Retorna número de peças fora do lugar em relação ao estado objetivo.
*/
    int dist = 0;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (G->grid[i][j]!=i*3+j)
                dist++;
        }
    return dist;
}

node greedySearch(game* G){
    //Inicializa primeiro nó com a configuração fornecida do jogo.
    node nd = childNode(NULL,0);
    copyGame(G,nd->state);
    if (endGame(G)){
        return nd;
    }
    //Armazena somente um nó, já que cada um possui um ponteiro para o anterior
    node parent = nd;

    while(parent!=NULL){
        //Variável auxiliar 
        game* aux = newGame();
        copyGame(parent->state,aux);
        //Ponteiro para armazenar o filho mais próximo do objetivo
        node optChild = NULL;
        //Para armazenar menor distância
        int dist = 1000;
        //Cria nós filhos para cada ação possível
        for (int k=1; k<9; k++){
            //Checa se movimento é válido
            if (moveGame(aux,k)){
                //Cria filho
                node auxChild = childNode(parent,k);
                //Se é a menor distância até aqui, armazena
                if (winDist(auxChild->state)<dist){
                    dist = winDist(auxChild->state);
                    optChild = auxChild;
                }
                //Caso contrário, libera memória
                else{
                    delNode(auxChild);
                }
                moveGame(aux,k);
            }
        }
        //Verifica se resolveu o problema
        if (endGame(optChild->state))
            return optChild;
        //Nó mais próximo do objetivo será o próximo a ser expandido
        parent = optChild;
        //sleep(1);
        //printGame(parent->state); printf("\n");
        //Libera memória da variável auxiliar
        delGame(aux);
    }
    return NULL;
}
