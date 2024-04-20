CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
SRC = main.c
EXEC = max_ascii_values

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

all: $(EXEC)

clean:
	rm -f $(EXEC)
	
.PHONY: all clean