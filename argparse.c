/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (682989)
 *
 * Please see the corresponding header file for documentation on the module.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argparse.h"

extern char *optarg;

Args parse_args(int argc, char *argv[]) {
    Args args;
    char input;

    // initialize args struct
    args.filename = NULL;
    args.algorithm = NULL;
    args.memsize = 0;
    args.quantum = 0;

    // parse and populate the struct using getopt
    while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF) {
        switch (input) {
            case 'f':
                args.filename = optarg;
                break;
            case 'a':
                args.algorithm = optarg;
                break;
            case 'm':
                args.memsize = strtol(optarg, NULL, 10);
                break;
            case 'q':
                args.quantum = strtol(optarg, NULL, 10);
                break;
            default:
                // just ignore any unrecognized arguments
                break;
        }
    }

    return args;
}
