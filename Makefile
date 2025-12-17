CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude
LDFLAGS = -lm

OBJ = codeC/main.o codeC/avl.o codeC/input.o codeC/output.o
EXEC = codeC/cwire

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

codeC/%.o: codeC/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) $(EXEC).exe *.csv *.png