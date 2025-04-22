#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../game/game.h"
#include "../exporter/exporter.h"

#define MAX_MOVES 1000          // Número máximo de movimentos possíveis
#define LIMIT 100               // Limite dos valores possíveis para solução
#define MAX_QUEUE_SIZE 10000    // Tamanho máximo da fila para a BFS

// Estrutura para armazenar o estado do jogo e o número de movimentos feitos
typedef struct {
    game* state;         // Estado do jogo
    int movesMade;       // Número de movimentos feitos
} bfsNode;

// Fila para a BFS
typedef struct {
    bfsNode nodes[MAX_QUEUE_SIZE]; // Array de nós
    int front;                      // Índice do primeiro elemento
    int rear;                       // Índice do próximo espaço livre
} Queue;

// Função para inicializar a fila
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

// Função para verificar se a fila está vazia
int isQueueEmpty(Queue* q) {
    return q->front == q->rear;
}

// Função para adicionar um nó à fila
void enqueue(Queue* q, bfsNode node) {
    if ((q->rear + 1) % MAX_QUEUE_SIZE == q->front) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    q->nodes[q->rear] = node;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
}

// Função para remover um nó da fila
bfsNode dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("Erro: Fila vazia!\n");
        exit(1);
    }
    bfsNode node = q->nodes[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return node;
}

// Função para gerar todos os movimentos válidos a partir do estado atual do jogo
void generateValidMoves(game* currentState, Queue* queue, int* visitedStates, int currentMoves) {
    for (int move = 0; move < LIMIT; move++) {
        game* newState = newGame();
        if (moveGame(currentState, move)) {
            copyGame(currentState, newState);

            // Verifica se o estado já foi visitado para evitar ciclos
            if (!visitedStates[getStateHash(newState)]) {
                visitedStates[getStateHash(newState)] = 1;

                bfsNode newNode;
                newNode.state = newState;
                newNode.movesMade = currentMoves + 1;  // Atualiza o número de movimentos
                enqueue(queue, newNode);
            }
        }
        delGame(newState);
    }
}

// Função BFS para encontrar a melhor solução (menor número de movimentos)
bool breadthFirstSearch(game* initial, int* finalMovesUsed) {
    Queue queue;
    initQueue(&queue);

    int visitedStates[MAX_MOVES] = { 0 };  // Array de estados visitados para evitar ciclos

    bfsNode startNode;
    startNode.state = newGame();
    copyGame(initial, startNode.state);
    startNode.movesMade = 0;  // Inicializa com zero movimentos
    enqueue(&queue, startNode);

    visitedStates[getStateHash(startNode.state)] = 1;  // Marca o estado inicial como visitado

    while (!isQueueEmpty(&queue)) {
        bfsNode currentNode = dequeue(&queue);

        // Verifica se a solução foi alcançada (estado de vitória)
        if (isGoalState(currentNode.state)) {
            *finalMovesUsed = currentNode.movesMade;
            delGame(currentNode.state);
            return true;  // Solução encontrada
        }

        // Gera todos os estados vizinhos e os adiciona à fila
        generateValidMoves(currentNode.state, &queue, visitedStates, currentNode.movesMade);
        delGame(currentNode.state);
    }

    return false;  // Não encontrou uma solução
}

void computeBreadthFirstSearch(game** games, char* filename) {
    int moves[10];
    double times[10];
    char victories[10];

    for (int i = 0; i < 10; i++) {
        int movesUsed = 0;
        game* initial = games[i];
        
        clock_t begin = clock();
        bool solved = breadthFirstSearch(initial, &movesUsed);
        clock_t end = clock();

        double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        moves[i] = movesUsed;
        times[i] = timeSpent;
        victories[i] = solved ? 'V' : 'D';

        delGame(initial);
    }

    exportFile(filename, moves, times, victories);
}
