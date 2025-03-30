#include "bfstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows

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

node BFS(game* G){
/*
    Breadth-First Search (Busca em largura).
*/
    //Inicializa primeiro nó com a configuração fornecida do jogo.
    node nd = childNode(NULL,0);
    copyGame(G,nd->state);
    if (endGame(G)){
        return nd;
    }
    printGame(G); printf("\n");

    //Inicializa fronteira e conjunto de nós já explorados
    fila frontier = newFila();
    insert(frontier,nd);
    fila explored = newFila();
    insert(explored,nd);
    
    while(frontier->first!=NULL){
        //Armazena último nó em frontier
        node parent = pop(frontier);
        //Variável auxiliar
        game* aux = newGame();
        copyGame(parent->state,aux);
        //Cria nós filhos para cada ação possível
        for (int k=1; k<9; k++){
            //Checa se movimento é válido
            if (moveGame(aux,k)){
                node child = childNode(parent,k);
                if (!isIn(explored,child)){
                    //Checa se nó filho resolve o problema
                    if (endGame(child->state)){
                        return child;
                    }
                    //Caso contrário, insere nó filho em frontier
                    insert(frontier,child);
                    //Insere em explored
                    insert(explored,child);
                    //Impressão na tela
                    //sleep(1);
                    //printGame(child->state); printf("\n");
                }
                else{ //para poupar memória
                    delNode(child);
                }
                //Retorna para estado anterior ao movimento
                moveGame(aux,k);
            }
        }
        //Gerenciamento adequado de memória. Não é necessário, mas eu fiz
        delGame(aux);
    }
    
    return NULL;
}