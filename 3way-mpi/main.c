#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define LINE_COUNT 750000

int max_char[LINE_COUNT]; // count of individual characters

int max_ascii_value(char *line) {
    int max_value = 0;
    while (*line) {
        if ((int)*line > max_value) {
            max_value = (int)*line;
        }
        line++;
    }
    return max_value;
}

void count_array(int startPos, int endPos) {
    FILE *file;
    int i;
    char line[LINE_COUNT]; // Max line length
    char *file_path = "/homes/dan/625/wiki_dump.txt"; 
    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file.\n");
            fclose(file);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    int line_number = startPos;
    while (fgets(line, LINE_COUNT, file) != NULL && line_number < endPos) {
        max_char[line_number] = max_ascii_value(line);
        line_number++;
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    int rank, size, startPos, endPos, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //printf("Number of threads: %d\n", size);
    startPos = rank * (LINE_COUNT / size);
    endPos = (rank + 1) * (LINE_COUNT / size);

    count_array(startPos, endPos);

    MPI_Allreduce(MPI_IN_PLACE, max_char, LINE_COUNT, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        for (i = 0; i < LINE_COUNT; i++) {
            printf("%d: %d\n", i, max_char[i]);
        }
    }

    MPI_Finalize();
    return 0;
}