/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The disk module. Stores process in queue but popping takes into account the
 * priority if there are ties for oldest process in queue.
 *
 */

#pragma once

#include "process.h"

/*
 * The Disk struct, internals are private to the module.
 */
typedef struct Disk Disk;

/*
 * Create a new empty disk instance.
 */
Disk *disk_init();

/*
 * Returns true if there are no processes on the disk.
 */
int disk_is_empty(Disk *disk);

/*
 * Adds the given process to the disk as being added at the given time.
 */
void disk_swap_in(Disk *disk, Process *proc, int time);

/*
 * Removes and returns the process that has been on the disk the longest.
 * If there are multiple such processes, the process with the highest priority
 * will be chosen.
 */
Process *disk_swap_out(Disk *disk);

/*
 * Used to deallocate the disk instance.
 */
void disk_destroy(Disk *disk);

/*
 * Print out the disk contents.
 * For use in debugging.
 */
void disk_print(Disk *disk);
