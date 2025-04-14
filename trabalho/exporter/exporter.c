#include <stdio.h>
#include <stdlib.h>

void exportFile(char *filename, int *movements, double *times) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(20);
        return;
    }

    // Write the integer array to the first line
    for (int i = 0; i < 10; i++) {
        fprintf(file, "%d", movements[i]);
        if (i < 9) {
            fprintf(file, "|");
        }
    }
    fprintf(file, "\n");

    // Write the double array to the second line
    for (int i = 0; i < 10; i++) {
        fprintf(file, "%.5f", times[i]);
        if (i < 9) {
            fprintf(file, "|");
        }
    }
    fprintf(file, "\n");

    fclose(file);
}
