#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define LINE_COUNT 500000

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
        exit(1);
    }
    for (i = 0; i < startPos; i++) {
        if (fgets(line, LINE_COUNT, file) == NULL) {
            printf("Error: startPos exceeds number of lines in file.\n");
            fclose(file);
            exit(1);
        }
    }
    int line_number = startPos;
    #pragma omp parallel for shared(max_char) private(line_number, line) schedule(static)
    for (line_number = startPos; line_number < endPos; line_number++) {
        if (fgets(line, LINE_COUNT, file) != NULL) {
            max_char[line_number] = max_ascii_value(line);
        }
    }
    fclose(file);
}

int main() {
    int startPos, endPos, i;

    startPos = 0;
    endPos = LINE_COUNT;

    #pragma omp parallel
    {
        count_array(startPos, endPos);
    }

    for (i = 0; i < LINE_COUNT; i++) {
        printf("%d: %d\n", i, max_char[i]);
    }

    return 0;
}
