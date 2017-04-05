/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (682989)
 *
 * TODO: write up a program description
 *
 */

#include <stdio.h>

#include "argparse.h"
#include "process.h"
#include "proc_creator.h"

#define TEST_ARGPARSE 0
#define TEST_PROC_CREATOR 1

#if TEST_ARGPARSE
int main(int argc, char *argv[]) {
    printf("Testing argparse\n");

    Args args = parse_args(argc, argv);

    printf(" filename: %s\n", args.filename);
    printf("algorithm: %s\n", args.algorithm);
    printf("  memsize: %d\n", args.memsize);
    printf("  quantum: %d\n", args.quantum);

    return 0;
}

#elif TEST_PROC_CREATOR
int main(int argc, char *argv[]) {
    printf("Testing proc_creator\n");

    Args args = parse_args(argc, argv);
    ProcCreator *pc = proc_creator_init(args.filename);
    if (pc == NULL) {
        printf("Unable to create ProcCreator instance.\n");
        return 1;
    }

    printf("Reading all processes from: %s\n", args.filename);

    int time = 0;
    while (!proc_creator_all_processes_released(pc) && time < 200) {
        if (proc_creator_has_next(pc, time)) printf("Time: %d\n", time);
        while (proc_creator_has_next(pc, time)) {
            printf("   ");
            process_print(proc_creator_get_next(pc, time));
            printf("\n");
        }
        time++;
    }

    return 0;
}

#else
int main(int argc, char *argv[]) {
    printf("Real main\n");

    return 0;
}

#endif
