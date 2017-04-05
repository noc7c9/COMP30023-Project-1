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
#include "linked_list.h"
#include "disk.h"

#define TEST_ARGPARSE 0
#define TEST_PROC_CREATOR 0
#define TEST_LINKED_LIST 0
#define TEST_DISK 1

#define TEST(title, expected, got) printf("%s: %s: ((expected)%d == (got)%d)\n", ((got) == (expected)) ? "passed" : "FAILED", title, (int)(expected), (int)(got))

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

#elif TEST_LINKED_LIST

int main() {
    printf("Testing linked_list\n");

    LinkedList *ll = linked_list_init();
    TEST("initial list empty", 1, linked_list_is_empty(ll));

    int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // push start on empty list
    linked_list_push_start(ll, &v[1]);
    TEST("push_start(_, _) increments len", 1, ll->len);
    TEST("push_start(empty, _) sets head", v[1], *(int *)ll->head->data);
    TEST("push_start(empty, _) sets tail", v[1], *(int *)ll->tail->data);

    // pop start on list with one node
    int popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_start(_, _) decrements len", 0, ll->len);
    TEST("pop_start(_) returns the right element", v[1], popped_value);
    TEST("pop_start(one_element) clears head", 1, NULL == ll->head);
    TEST("pop_start(one_element) clears tail", 1, NULL == ll->tail);

    // push end on empty list
    linked_list_push_end(ll, &v[1]);
    TEST("push_end(_, _) increments len", 1, ll->len);
    TEST("push_end(empty, _) sets head", v[1], *(int *)ll->head->data);
    TEST("push_end(empty, _) sets tail", v[1], *(int *)ll->tail->data);

    // pop end on list with one node
    popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_end(_, _) decrements len", 0, ll->len);
    TEST("pop_end(_) returns the right element", v[1], popped_value);
    TEST("pop_end(one_element) clears head", 1, NULL == ll->head);
    TEST("pop_end(one_element) clears tail", 1, NULL == ll->tail);

    // push multiple values
    linked_list_push_start(ll, &v[2]); // list == [v2]
    linked_list_push_end(ll, &v[3]);   // list == [v2, v3]
    linked_list_push_start(ll, &v[1]); // list == [v1, v2, v3]
    TEST("check multiple value 1", v[1], *(int *)ll->head->data);
    TEST("check multiple value 2", v[2], *(int *)ll->head->next->data);
    TEST("check multiple value 3", v[3], *(int *)ll->head->next->next->data);
    TEST("check multiple len is correct", 3, ll->len);

    // removing values from the middle
    linked_list_pop(ll, ll->head->next); // list == [v1, v2, v3] => [v1, v3]
    TEST("check multiple value post middle pop 1", v[1], *(int *)ll->head->data);
    TEST("check multiple value post middle pop 2", v[3], *(int *)ll->head->next->data);
    TEST("check multiple len is correct post middle pop", 2, ll->len);
}

#elif TEST_DISK
int main() {
    printf("Testing disk\n");

    Disk *disk = disk_init();
    TEST("initial disk is empty", 1, disk_is_empty(disk));

    disk_swap_in(disk, process_init(-1, 2, -1, -1), 0);
    disk_swap_in(disk, process_init(-1, 1, -1, -1), 0);
    disk_swap_in(disk, process_init(-1, 0, -1, -1), 1);

    // disk_print(disk); printf("\n");

    Process *proc = disk_swap_out(disk);
    TEST("process swapped out check 1", 1, proc->process_id);
    proc = disk_swap_out(disk);
    TEST("process swapped out check 2", 2, proc->process_id);
    proc = disk_swap_out(disk);
    TEST("process swapped out check 3", 0, proc->process_id);

    TEST("disk should now be empty again", 1, disk_is_empty(disk));
}

#else
int main(int argc, char *argv[]) {
    printf("Real main\n");

    return 0;
}

#endif
