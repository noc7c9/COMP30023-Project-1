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

#define TEST_ARGPARSE 0
#define TEST_PROC_CREATOR 0
#define TEST_LINKED_LIST 1

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

#elif TEST_LINKED_LIST

#define TEST(title, condition) printf(title ": %s\n", (condition) ? "passed" : "FAILED")

int main() {
    printf("Testing linked_list\n");

    LinkedList *ll = linked_list_init();
    TEST("initial list empty", linked_list_is_empty(ll));

    int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // push start on empty list
    linked_list_push_start(ll, &v[1]);
    TEST("push_start(_, _) increments len", ll->len == 1);
    TEST("push_start(empty, _) sets head", *(int *)ll->head->data == v[1]);
    TEST("push_start(empty, _) sets tail", *(int *)ll->tail->data == v[1]);

    // pop start on list with one node
    int popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_start(_, _) decrements len", ll->len == 0);
    TEST("pop_start(_) returns the right element", popped_value == v[1]);
    TEST("pop_start(one_element) clears head", ll->head == NULL);
    TEST("pop_start(one_element) clears tail", ll->tail == NULL);

    // push end on empty list
    linked_list_push_end(ll, &v[1]);
    TEST("push_end(_, _) increments len", ll->len == 1);
    TEST("push_end(empty, _) sets head", *(int *)ll->head->data == v[1]);
    TEST("push_end(empty, _) sets tail", *(int *)ll->tail->data == v[1]);

    // pop end on list with one node
    popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_end(_, _) decrements len", ll->len == 0);
    TEST("pop_end(_) returns the right element", popped_value == v[1]);
    TEST("pop_end(one_element) clears head", ll->head == NULL);
    TEST("pop_end(one_element) clears tail", ll->tail == NULL);

    // push multiple values
    linked_list_push_start(ll, &v[2]); // list == [v2]
    linked_list_push_end(ll, &v[3]);   // list == [v2, v3]
    linked_list_push_start(ll, &v[1]); // list == [v1, v2, v3]
    TEST("push multiple value 1", *(int *)ll->head->data == v[1]);
    TEST("push multiple value 2", *(int *)ll->head->next->data == v[2]);
    TEST("push multiple value 3", *(int *)ll->head->next->next->data == v[3]);
    TEST("push multiple len is correct", ll->len == 3);
}

#else
int main(int argc, char *argv[]) {
    printf("Real main\n");

    return 0;
}

#endif
