/**
 * @file mpi_max_ascii_values.c
 * @brief Multi-threaded program to find the maximum ASCII value in each line of a text file.
 * @details Uses MPI to process the file concurrently.
 * Was informed by TA that implementation is fine the way it is
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define LINE_COUNT 1000000
#define MAX_LINE_SIZE 5000

int max_char[LINE_COUNT]; // Array to store the maximum ASCII value of each line

/**
 * @brief Calculates the maximum ASCII value in a given line.
 * @param line The input line.
 * @return The maximum ASCII value found in the line.
 */
int max_ascii_value(char *line) {
    int max_value = 0;
    //Check to see if we are still going through each character in the line
    while (*line) {
        if ((int)*line > max_value) {
            max_value = (int)*line;
        }
        line++;
    }
    return max_value;
}

/**
 * @brief Reads and processes a portion of the input file.
 * @param startPos The starting position for processing lines in the file.
 * @param endPos The ending position for processing lines in the file.
 */
void count_array(int startPos, int endPos) {
    FILE *file;
    int i;
    char line[LINE_COUNT]; // Array to store a line from the file
    char *file_path = "/homes/dan/625/wiki_dump.txt"; 

    // Open the file in read mode
    file = fopen(file_path, "r");
    if (file == NULL) { //Check to see if we successfully opened the file, if not abort the process
        printf("Error opening file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    //Checks to see if the starting position is outside the bounds of the lines of the file
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file.\n");
            fclose(file);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    //The primary section of count_array that gets the max ascii value for each line (handled by MPI code for critical section)
    int line_number = startPos;
    while (fgets(line, MAX_LINE_SIZE, file) != NULL && line_number < endPos) {
        max_char[line_number] = max_ascii_value(line);
        line_number++;
    }
    fclose(file);
}


int main(int argc, char *argv[]) {
    int rank, size, startPos, endPos, i;

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    startPos = rank * (LINE_COUNT / size); // Calculate the start position for this process
    endPos = (rank + 1) * (LINE_COUNT / size); // Calculate the end position for this process


    count_array(startPos, endPos); // Process lines from the file for this process

    // Combine results from all processes to get the overall maximum ASCII values
    MPI_Allreduce(MPI_IN_PLACE, max_char, LINE_COUNT, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // Print the overall maximum ASCII values for each line 
    // since we have finished getting the values
    if (rank == 0) {
        for (i = 0; i < LINE_COUNT; i++) {
            printf("%d: %d\n", i, max_char[i]);
        }
    }

    //Finalize the environment, and exit
    MPI_Finalize(); 
    return 0;
}