#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

double getNewSolution(double current, double range);
double simulatedAnnealing (double (*func)(double));
bool acceptSolution(double temp, double energyDiff);