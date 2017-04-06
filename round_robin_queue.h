/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The very simple round robin queue module.
 *
 */

#pragma once

#include "process.h"

/*
 * The Queue struct, internals are private to the module.
 */
typedef struct RRQueue RRQueue;

/*
 * Create a new empty queue instance.
 */
RRQueue *rrqueue_init();

/*
 * Returns true if there are no processes in the queue.
 */
int rrqueue_is_empty(RRQueue *rrqueue);

/*
 * Returns (without removing) the process that's at the front of the queue.
 */
Process *rrqueue_peek(RRQueue *rrqueue);

/*
 * Adds the given process to the end of the queue.
 */
void rrqueue_push(RRQueue *rrqueue, Process *proc);

/*
 * Removes and returns the process that's at the front of the queue.
 */
Process *rrqueue_pop(RRQueue *rrqueue);

/*
 * Removes the given process from the queue.
 * Returns NULL if the process wasn't in the queue.
 */
Process *rrqueue_remove(RRQueue *rrqueue, Process *proc);

/*
 * Used to deallocate the given RRQueue instance.
 */
void rrqueue_destroy(RRQueue *rrqueue);

/*
 * Print out the queue contents.
 * For use in debugging.
 */
void rrqueue_print(RRQueue *rrqueue);
