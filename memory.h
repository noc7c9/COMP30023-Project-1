/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The memory module. Uses two linked lists, one to represent the memory and
 * another to that has the processes in memory.
 *
 * Also contains the first/best/worst fit algorithm implementations.
 *
 */

#pragma once

#include "process.h"

/*
 * The Memory struct, internals are private to the module.
 */
typedef struct Memory Memory;

/*
 * Create a new empty memory instance of the given size.
 * Also takes the memory hole selection algorithm.
 * (one of "first", "best" or "worst")
 */
Memory *memory_init(int size, char *algorithm);

/*
 * Returns true if there are no processes in memory.
 */
int memory_is_empty(Memory *mem);

/*
 * Adds the given process to memory.
 * Returns true if the process was successfully added to memory, and false
 * otherwise. It can fail if there isn't a continuous chunk of memory large
 * enough to store the given process.
 */
int memory_swap_in(Memory *mem, Process *proc);

/*
 * Removes and returns the given process from memory.
 */
Process *memory_swap_out_process(Memory *mem, Process *proc);

/*
 * Removes and returns the process that has been in memory the longest.
 */
Process *memory_swap_out_oldest(Memory *mem);

/*
 * Returns the number of processes in memory.
 */
int memory_numprocesses(Memory *mem);

/*
 * Returns the number of free chunks in memory.
 */
int memory_numholes(Memory *mem);

/*
 * Returns the percentage of in-used memory.
 */
double memory_memusage(Memory *mem);

/*
 * Used to deallocate the memory instance.
 */
void memory_destroy(Memory *mem);

/*
 * Print out the memory contents.
 * For use in debugging.
 */
void memory_print(Memory *mem);
