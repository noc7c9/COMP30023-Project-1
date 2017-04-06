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

#include "process.h"

Process *process_init(int time_created, int process_id, int memory_size, int job_time) {
    Process *proc = (Process*)malloc(sizeof(Process));
    assert(proc);

    proc->time_created = time_created;
    proc->process_id = process_id;
    proc->memory_size = memory_size;
    proc->job_time = job_time;

    // a lower process_id is considered a higher priority
    proc->priority = -process_id;

    return proc;
}

void process_destroy(Process *proc) {
    free(proc);
}

void process_print(Process *proc) {
    printf("Proc(time_created=%d, process_id=%d, memory_size=%d, job_time=%d)",
            proc->time_created,
            proc->process_id,
            proc->memory_size,
            proc->job_time);
}
