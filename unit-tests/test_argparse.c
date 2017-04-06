int main(int argc, char *argv[]) {
    printf("Testing argparse\n");

    Args args = parse_args(argc, argv);

    printf(" filename: %s\n", args.filename);
    printf("algorithm: %s\n", args.algorithm);
    printf("  memsize: %d\n", args.memsize);
    printf("  quantum: %d\n", args.quantum);

    return 0;
}
