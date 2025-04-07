#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

// Estrutura para armazenar uma solução e seu valor
typedef struct {
    double solution;
    double value;
} Candidate;

double getNewSolution(double current, double range);
int compareCandidates(const void *a, const void *b);
double localBeamSearch(double (*func)(double));