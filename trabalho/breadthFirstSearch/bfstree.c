#include "bfstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include "../exporter/exporter.h"


fila newFila(){
    fila f = malloc(sizeof(Fila));
    assert(f);
    f->first = NULL;
    f->last = NULL;
    return f;
}

void insert(fila f, node newNode){
/*
    Insere newNode na última posição de f
*/
    item I = malloc(sizeof(struct Item));
    assert(I);
    I->nd = newNode;
    I->next = NULL;
    if (f->last==NULL){
        assert(f);
        f->first = I;
        f->last = I;
    }
    else{
        f->last->next = I;
        f->last = I;
    }
}

node pop(fila f){
/*
    Retorna o primeiro nó da fila, removendo-o da mesma.
*/
    item I = f->first;
    node nd = I->nd;
    f->first = I->next;
    if (f->first==NULL){
        f->last = NULL;
    }
    free(I);
    return nd;
}

bool isIn(fila f, node nd){
/*
    Checa se nd está na fila comparando seu estado com o dos demais.
*/
    for (item aux=f->first; aux!=NULL; aux=aux->next){
        if (equal(aux->nd->state,nd->state))
            return true;
    }
    return false;
}

int lenFila(fila f){
/*
    Número de elementos na fila. Foi implementado para rodar alguns testes.
*/
    int k = 0;
    for (item aux=f->first; aux!=NULL; aux=aux->next){
        k++;
    }
    return k;
}

node BFS(game* G, int* finalMovesUsed){

    node nd = childNode(NULL, 0);
    copyGame(G, nd->state);

    if (endGame(G)) {
        *finalMovesUsed = 0;
        return nd;
    }

    printGame(G); printf("\n");

    fila frontier = newFila();
    insert(frontier, nd);
    fila explored = newFila();
    insert(explored, nd);
    
    while (frontier->first != NULL) {
        // Armazena último nó em frontier
        node parent = pop(frontier);


        // Variável auxiliar
        game* aux = newGame();
        copyGame(parent->state, aux);

        // Cria nós filhos para cada ação possível
        for (int k = 1; k < 9; k++) {
            if (moveGame(aux, k)) {
                node child = childNode(parent, k);
                if (!isIn(explored, child)) {
                    if (endGame(child->state)) {
                        *finalMovesUsed = child->depth;  // <- Aqui contamos os movimentos
                        delGame(aux);  // libera memória antes de retornar
                        return child;
                    }
                    insert(frontier, child);
                    insert(explored, child);
                } else {
                    delNode(child);  // para poupar memória
                }
                moveGame(aux, k);  // volta ao estado anterior
            }
        }
        delGame(aux);
    }

    return NULL; // Não encontrou solução
}

void computeBFS(game** games, char* filename) {
    int moves[10];
    double times[10];
    char victories[10];

    for (int i = 0; i < 10; i++) {
        int movesUsed = 0;
        game* initial = games[i];

        clock_t begin = clock();
        node solution = BFS(initial, &movesUsed);
        clock_t end = clock();

        double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        moves[i] = movesUsed;
        times[i] = timeSpent;
        victories[i] = (solution != NULL) ? 'V' : 'D';

        if (solution != NULL) {
            delNode(solution);
        }

        delGame(initial);  // libera o estado inicial
    }

    exportFile(filename, moves, times, victories);
}

