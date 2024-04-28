/**
 * @file mpi_max_ascii_values.c
 * @brief Multi-threaded program to find the maximum ASCII value in each line of a text file using OpenMP.
 * @details This program uses OpenMP to process the file concurrently with multiple threads.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

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
    if (file == NULL) { //Check to see if we successfully opened the file, if not, exit
        printf("Error opening file.\n");
        exit(1);
    }

    //Checks to see if the starting position is outside the bounds of the lines of the file
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file.\n");
            fclose(file);
            exit(1);
        }
    }

    //Primary section of count_array where the max ascii value is calculated
    int line_number;
    for (line_number = startPos; line_number < endPos; line_number++) {
        if (fgets(line, MAX_LINE_SIZE, file) != NULL) {
            max_char[line_number] = max_ascii_value(line);
        }
    }

    //Close the file upon completion
    fclose(file);
}

int main() {
    int i;

    #pragma omp parallel // Start parallel region with OpenMP
    {
        int numThreads = omp_get_num_threads(); // Get the total number of threads
        int threadID = omp_get_thread_num(); // Get the thread ID
        int linesPerThread = LINE_COUNT / numThreads; // Calculate the number of lines per thread
        int startPos = threadID * linesPerThread; // Calculate the starting position for this thread
        int endPos = (threadID == numThreads - 1) ? LINE_COUNT : startPos + linesPerThread; // Calculate the ending position for this thread

        count_array(startPos, endPos); // Process lines from the file for this thread
    }

    // Print the overall maximum ASCII values for each line
    for (i = 0; i < LINE_COUNT; i++) {
        printf("%d: %d\n", i, max_char[i]);
    }

    return 0; // Exit the program successfully
}