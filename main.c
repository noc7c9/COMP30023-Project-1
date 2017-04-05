#include <stdio.h>

#include "argparse.h"

#define TEST_ARGPARSE 1

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

#else
int main(int argc, char *argv[]) {
    printf("Real main\n");

    return 0;
}

#endif
