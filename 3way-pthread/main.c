/**
 * @file max_ascii_values.c
 * @brief Multi-threaded program to find the maximum ASCII value in each line of a text file.
 * @details Uses pthreads to process the file concurrently.
 * Was informed by TA that implementation is fine the way it is
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 20 // Number of threads to create
#define LINE_COUNT 1000000 // Number of lines to read from the file
#define MAX_LINE_SIZE 5000
pthread_mutex_t mutexsum; // Mutex for synchronization
int max_char[LINE_COUNT]; // Array to store max ASCII value of each line

/**
 * @brief Finds the maximum ASCII value in a given line.
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
 * @brief Thread function to count the max ASCII value for lines in a file segment.
 * @param myID The thread ID.
 */
void *count_array(void *myID){
    FILE *file;
    int i;
    char line[LINE_COUNT]; // Max line length
    char *file_path = "/homes/dan/625/wiki_dump.txt"; 
    int startPos = ((long) myID) * (LINE_COUNT / NUM_THREADS);
    int endPos = startPos + (LINE_COUNT / NUM_THREADS); 

    // Open the file in read mode
    file = fopen(file_path, "r");
    if (file == NULL) { //Check to see if we successfully opened the file
        printf("Error opening file in thread: %ld \n", (long) myID);
        return NULL;
    }


    //Checks to see if the starting position is outside the bounds of the lines of the file
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file. Thread: %ld \n", (long) myID);
            fclose(file);
            return NULL;
        }
    }

    //The critical section of the count_array method:
    //This is where it starts going through the specific chunks of lines and getting the max ascii value
    pthread_mutex_lock (&mutexsum);
    int line_number = startPos;
    while (fgets(line, MAX_LINE_SIZE, file) != NULL && line_number < endPos) {
        max_char[line_number] = max_ascii_value(line);
        line_number++;
    }
    //Unlock the mutex once done computing the max char for the specific chunk, close the file, and exit the thread
    pthread_mutex_unlock (&mutexsum);
    fclose(file);
    pthread_exit(NULL);
}

/**
 * @brief Initializes the max_char array and mutex.
 */
void init_arrays()
{
    int i;
    pthread_mutex_init(&mutexsum, NULL);
    for (i = 0; i < LINE_COUNT; i++ ) {
    max_char[i] = 0;
    }
}

/**
 * @brief Prints the results, i.e., the max ASCII values for each line.
 */
void print_results()
{
    int i;
    for (i = 0; i < LINE_COUNT; i++) {
        printf("%d: %d\n", i, max_char[i]);
    }
}

/**
 * @brief Main function to create and join threads for processing the file.
 * @return 0 on success.
 */
int main() {
    pthread_t threads[NUM_THREADS]; // Array to store thread IDs
	pthread_attr_t attr; // Thread attributes
	void *status; // Status pointer for thread join
	int i, rc; // Loop variable and return code

    //Initialize the thread attributes, set threads so they can join to other threads, then initialize the arrays
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	init_arrays();

    // Create threads to process file chunks concurrently
    for (i = 0; i < NUM_THREADS; i++ ) {
	      rc = pthread_create(&threads[i], &attr, count_array, (void *)(long)i);
	      if (rc) {
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	      }
	}


	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);

    // Wait for all threads to complete before continuing
	for(i=0; i<NUM_THREADS; i++) {
	     rc = pthread_join(threads[i], &status); //Join the threads once finished
	     if (rc) { //If an error occurs when threads finish
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}

    print_results(); // Print the results of processing the file segments
	pthread_mutex_destroy(&mutexsum); // Destroy the mutex used for synchronization
	pthread_exit(NULL); // Exit the main thread
	return 0; // Exit the program with success code
}