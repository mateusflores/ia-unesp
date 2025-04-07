#include "shill.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

double quad(double x){
    return x * x;
}

double rastrigin(double x){
    return 10 + x * x - 10 * cos(2 * PI * x);
}