#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4
#define LINE_COUNT 1000000
//pthread_mutex_t mutexsum;			// mutex for char_counts
int max_char[LINE_COUNT];			// count of individual characters

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

void *count_array(void *myID){
    FILE *file;
    int i;
    char line[LINE_COUNT]; // Max line length
    char *file_path = "/homes/dan/625/wiki_dump.txt"; 
    
    int startPos = ((long) myID) * (LINE_COUNT / NUM_THREADS);
    int endPos = startPos + (LINE_COUNT / NUM_THREADS); //printf("myID = %ld startPos = %d endPos = %d \n", (long) myID, startPos, endPos);
    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file in thread: %ld \n", (long) myID);
        return NULL;
    }
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file. Thread: %ld \n", (long) myID);
            fclose(file);
            return NULL;
        }
    }
    //pthread_mutex_lock (&mutexsum);
    int line_number = startPos;
    while (fgets(line, LINE_COUNT, file) != NULL && line_number < endPos) {
        max_char[line_number] = max_ascii_value(line);
        line_number++;
    }
   // pthread_mutex_unlock (&mutexsum);
    //pthread_exit(NULL);
}

void init_arrays()
{
    int i;
   // pthread_mutex_init(&mutexsum, NULL);
    for (i = 0; i < LINE_COUNT; i++ ) {
    max_char[i] = 0;
    }
}

void print_results()
{
    int i;
    for (i = 0; i < LINE_COUNT; i++) { // change while back to line_count
        printf("%d: %d\n", i, max_char[i]);
    }
}

int main(int argc, char* argv[]) {
    int i, rc;
	int numtasks, rank;
	MPI_Status Status;


	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	    printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//NUM_THREADS = numtasks;
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);

	if ( rank == 0 ) {
		init_arrays();
	}
	MPI_Bcast(max_char, LINE_COUNT, MPI_CHAR, 0, MPI_COMM_WORLD);
		
	count_array(&rank);

	MPI_Reduce(max_char, LINE_COUNT, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


	if ( rank == 0 ) {
		print_results();
	}

	MPI_Finalize();
	return 0;
}