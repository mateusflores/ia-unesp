#include "localBeamSearch.h"
#include "shill.h"

/*
    Grupo:
    - Nome: Guilherme Caetano
    - Nome: Leonardo Cardoso
    - Nome: Mateus Flores
    - Nome: Vitor Luz
*/

int main(){

    int opt = 0;

    printf("Escolha uma opcao:\n\t1 - Funcao quadratica\n\t2 - Funcao rastrigin\n\t0 - Sair\n");
    scanf("%d",&opt);

    if (opt == 0)
        exit(2);
    else if (opt == 1) {
        double solution = localBeamSearch(quad);
        printf("Melhor solucao encontrada: %f\n", solution);
        printf("Valor da funcao objetivo: %f\n", 0);
    }
    else if (opt == 2) {
        double solution = localBeamSearch(rastrigin);
        printf("Melhor solucao encontrada: %f\n", solution);
        printf("Valor da funcao objetivo: %f\n", 0);
    }
    else {
        printf("Opcao invalida. Tente novamente.\n");
        exit(3);
    }

    return 0;
}