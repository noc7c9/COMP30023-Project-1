/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * The module defines the Process object, which is the data structure used to
 * represent the simulated process in all other parts of the program.
 *
 */

#pragma once

/*
 * The main process struct.
 */
typedef struct {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;
    int priority;
} Process;

/*
 * Used to create (allocate) a new process instance.
 */
Process *process_init(int time_created, int process_id, int memory_size, int job_time);

/*
 * Used to deallocate the given process instance.
 */
void process_destroy(Process *proc);

/*
 * Will print out the details of the given process instance.
 * For use in debugging.
 */
void process_print(Process *proc);
