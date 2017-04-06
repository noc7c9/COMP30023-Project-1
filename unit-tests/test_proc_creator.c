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
    while (!proc_creator_all_processes_released(pc)) {
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
