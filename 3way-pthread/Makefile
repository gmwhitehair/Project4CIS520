CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
SRC = main.c
EXEC = max_ascii_values

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXEC)
	
.PHONY: all clean