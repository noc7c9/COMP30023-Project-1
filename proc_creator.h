/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The module responsible for reading the process descriptions from the file,
 * creating the individual process instances and finally releasing them to the
 * simulation at the appropriate times.
 *
 * The module uses a very simple stream-like interface that releases processes
 * one by one as necessary.
 *
 */

#pragma once

#include "process.h"

/*
 * The ProcCreator struct, internals are private to the module.
 */
typedef struct ProcCreator ProcCreator;

/*
 * Returns a new ProcCreator instance that will read from the given file.
 *
 * If there was a problem (eg: file doesn't exist) the function will return
 * NULL.
 */
ProcCreator *proc_creator_init(char *filepath);

/*
 * The function that is used to get the process instances.
 * The function takes the current time of the simulation and will only release
 * the process if appropriate.
 *
 * Returns the next process instance if there is one and NULL otherwise.
 */
Process *proc_creator_get_next(ProcCreator *pc, int time);

/*
 * Returns true if the next process (if any) is to be released at the given time
 * and false otherwise.
 */
int proc_creator_has_next(ProcCreator *pc, int time);

/*
 * Returns true if all the processes have been released
 * and false if there are more processes to be read from the file.
 */
int proc_creator_all_processes_released(ProcCreator *pc);

/*
 * Used to deallocate a ProcCreator.
 */
void proc_creator_destroy(ProcCreator *pc);
