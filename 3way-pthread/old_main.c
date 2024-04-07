#include <stdio.h>
#include <stdlib.h>


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

int main() {
    FILE *file;
    char *file_path = "/homes/dan/625/wiki_dump.txt"; // Update with the correct file path
    char line[1000000];
    int line_count = 1000000;
    int *max_values = (int *)malloc(line_count * sizeof(int));
    printf("NON THREADED VERSION.\n");
    printf("********************\n");

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int line_number = 0;
    while (fgets(line, line_count, file) != NULL) {
        max_values[line_number] = max_ascii_value(line);
        line_number++;
    }

    for (int i = 0; i < line_count; i++) {
        printf("%d: %d\n", i, max_values[i]);
    }

    // Free dynamically allocated memory
    free(max_values);

    // Close the file
    fclose(file);
    printf("********************\n");
    printf("NON THREADED VERSION.\n");
    return 0;
}