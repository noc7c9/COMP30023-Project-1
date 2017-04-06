/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * Please see the corresponding header file for documentation on the module.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "linked_list.h"

#include "round_robin_queue.h"

struct RRQueue {
    LinkedList *processes;
};

RRQueue *rrqueue_init() {
    RRQueue *rrqueue = (RRQueue*)malloc(sizeof(RRQueue));
    assert(rrqueue);

    rrqueue->processes = linked_list_init();

    return rrqueue;
}

int rrqueue_is_empty(RRQueue *rrqueue) {
    return linked_list_is_empty(rrqueue->processes);
}

Process *rrqueue_peek(RRQueue *rrqueue) {
    if (rrqueue->processes->head != NULL) {
        return (Process *)rrqueue->processes->head->data;
    } else {
        return NULL;
    }
}

void rrqueue_push(RRQueue *rrqueue, Process *proc) {
    linked_list_push_end(rrqueue->processes, proc);
}

Process *rrqueue_pop(RRQueue *rrqueue) {
    return linked_list_pop_start(rrqueue->processes);
}

Process *rrqueue_remove(RRQueue *rrqueue, Process *proc) {
    Node *node = rrqueue->processes->head;
    while (node) {
        if (proc == (Process*)node->data) {
            // found!

            // now remove it
            return linked_list_pop(rrqueue->processes, node);
        }
        node = node->next;
    }

    // no matching process found
    return NULL;
}

/*
 * Helper to print out Process instances when printing the queue.
 */
void _print_process(void *pointer) {
    process_print(pointer);
    printf("\n");
}

void rrqueue_print(RRQueue *rrqueue) {
    printf("RRQueue");
    linked_list_print(rrqueue->processes, _print_process);
}
