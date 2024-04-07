#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4
#define LINE_COUNT 1000000
pthread_mutex_t mutexsum;			// mutex for char_counts
int max_char[LINE_COUNT];			// count of individual characters
struct thread_args {
    char *file_path;
    int start_pos;
    int end_pos;
    long thread_id;
};
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

void *count_array(void *arg){
    FILE *file;
    char line[1000000]; // Max line length
    struct thread_args *t_args = (struct thread_args *) arg;

    printf("myID = %ld startPos = %d endPos = %d \n", t_args->thread_id, t_args->start_pos, t_args->end_pos);

    file = fopen(t_args->file_path, "r");
    if (file == NULL) {
        printf("Error opening file in thread: %ld \n", (long) t_args->thread_id);
        return NULL;
    }

    fseek(file, t_args->start_pos * sizeof(char), SEEK_SET);


    pthread_mutex_lock (&mutexsum);
    // Read lines until reaching endPos or end of file
    int line_number = t_args->start_pos;
	int end_pos = t_args->end_pos;
    while (fgets(line, LINE_COUNT, file) != NULL && line_number < end_pos) {
        max_char[line_number] = max_ascii_value(line);
        line_number++;
    }

    pthread_mutex_unlock (&mutexsum);
    pthread_exit(NULL);

}

void init_arrays()
{
    int i;
    pthread_mutex_init(&mutexsum, NULL);
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

int main() {
    //FILE *file;
    char *file_path = "/homes/dan/625/wiki_dump.txt"; 
    pthread_t threads[NUM_THREADS];
	struct thread_args t_args[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	int i, rc;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	init_arrays();

    for (i = 0; i < NUM_THREADS; i++ ) {
		t_args[i].file_path = file_path;
		t_args[i].start_pos = i * (LINE_COUNT / NUM_THREADS);
		t_args[i].end_pos = (i == NUM_THREADS - 1)? LINE_COUNT : (i + 1) * (LINE_COUNT / NUM_THREADS);
		t_args[i].thread_id = i;
        rc = pthread_create(&threads[i], &attr, count_array, &t_args[i]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
	     rc = pthread_join(threads[i], &status);
	     if (rc) {
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}

	print_results();

	pthread_mutex_destroy(&mutexsum);
	printf("********************\n");
    printf("THREADED VERSION.\n");
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
	return 0;
}