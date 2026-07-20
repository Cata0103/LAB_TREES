CC = gcc
# CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = tree_exe
SRC = main.c node_tree.c leaf_tree.c array.c
OBJ = main.o node_tree.o leaf_tree.o array.o
LDLIBS = -lm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDLIBS)

main.o: main.c
	$(CC) -c main.c
node_tree.o: node_tree.c node_tree.h
	$(CC) -c node_tree.c
leaf_tree.o: leaf_tree.c leaf_tree.h
	$(CC) -c leaf_tree.c
array.o: array.c array.h
	$(CC) -c array.c
clean:
	rm -f *.o tree_exe

.PHONY: all clean