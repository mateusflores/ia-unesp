#include "simulatedAnnealing.h"
#include "shill.h"

/*
    Grupo:
    - Nome: Guilherme Caetano
    - Nome: Leonardo Cardoso
    - Nome: Mateus Flores
    - Nome: Vitor Luz

    - Código feito baseado no algoritmo de Simulated Annealing encontrado em: https://www.baeldung.com/cs/simulated-annealing
*/

int main(){

    int opt = 0;

    printf("Escolha uma opcao:\n\t1 - Funcao quadratica\n\t2 - Funcao rastrigin\n\t0 - Sair\n");
    scanf("%d",&opt);

    if (opt == 0)
        exit(2);
    else if (opt == 1) {
        double solution = simulatedAnnealing(quad);
        printf("Melhor solucao encontrada: %f\n", solution);
        printf("Valor da funcao objetivo: %f\n", 0);
    }
    else if (opt == 2) {
        double solution = simulatedAnnealing(rastrigin);
        printf("Melhor solucao encontrada: %f\n", solution);
        printf("Valor da funcao objetivo: %f\n", 0);
    }
    else {
        printf("Opcao invalida. Tente novamente.\n");
        exit(3);
    }

    return 0;
}