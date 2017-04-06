/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The module responsible for parsing the arguments to the program.
 *
 */

#pragma once

/*
 * The container struct for the arguments.
 */
typedef struct {
    char *filename;
    char *algorithm;
    int memsize;
    int quantum;
} Args;

/*
 * Takes in the raw program arguments and parses it into an Args object.
 */
Args parse_args(int argc, char *argv[]);
