#
# Computer Systems Project 1 Makefile
# Ibrahim Athir Saleem (682989)
#

CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

OBJ = main.o argparse.o
EXE = swap

## Top level target is executable.
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

## Clean: Remove object files and core dump files.
clean:
	rm -f $(OBJ)

## Clobber: Performs Clean and removes executable file.
clobber: clean
	rm -f $(EXE)

run: $(EXE)
	./$(EXE) -f testdata/input.txt -a first -m 1000 -q 7

test: $(EXE)
	./$(EXE) -f testdata/input.txt -a first -m 1000 -q 7 | diff - testdata/output.txt
	./$(EXE) -f testdata/testFirst1 -a first -m 1000 -q 7 | diff - testdata/testFirst1.out
	./$(EXE) -f testdata/testBest1 -a best -m 1000 -q 7 | diff - testdata/testBest1.out
	./$(EXE) -f testdata/testWorst1 -a worst -m 1000 -q 7 | diff - testdata/testWorst1.out

## Dependencies
main.o: argparse.h
