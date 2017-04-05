/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (682989)
 *
 * Please see the corresponding header file for documentation on the module.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "proc_creator.h"

void _ready_next_process(ProcCreator *pc);

struct ProcCreator {
    FILE *fpointer;
    Process *next_proc;
};

ProcCreator *proc_creator_init(char *filepath) {
    ProcCreator *pc = (ProcCreator*)malloc(sizeof(ProcCreator));
    assert(pc);

    pc->fpointer = fopen(filepath, "r");

    // abort if opening the file failed
    if (pc->fpointer == NULL) {
        free(pc);
        return NULL;
    }

    // ready the first process for consumption
    _ready_next_process(pc);

    return pc;
}

Process *proc_creator_get_next(ProcCreator *pc, int time) {
    // if there is a process to be released, release it
    if (proc_creator_has_next(pc, time)) {
        Process *proc = pc->next_proc;
        _ready_next_process(pc);
        return proc;
    } else {
    // otherwise return nothing
        return NULL;
    }
}

int proc_creator_has_next(ProcCreator *pc, int time) {
    return pc->next_proc != NULL
        && pc->next_proc->time_created == time;
}

int proc_creator_all_processes_released(ProcCreator *pc) {
    return pc->next_proc == NULL;
}

/*
 * Private helper function to parse the next line from the file into a process,
 * and ready if for consumption.
 */
void _ready_next_process(ProcCreator *pc) {
    int time_created, process_id, memory_size, job_time;
    int read_values;

    // read the next set of values from the file
    read_values = fscanf(pc->fpointer, "%d %d %d %d",
            &time_created, &process_id, &memory_size, &job_time);

    if (read_values != 4) {
        // handle reaching the end of the file
        pc->next_proc = NULL;
    } else {
        // otherwise instantiate a new Process instance
        pc->next_proc = process_init();

        // and set its values
        pc->next_proc->time_created = time_created;
        pc->next_proc->process_id = process_id;
        pc->next_proc->memory_size = memory_size;
        pc->next_proc->job_time = job_time;
    }
}
