/*
 * COMP30023 Computer Systems Project 1
 * Ibrahim Athir Saleem (isaleem) (682989)
 *
 * Memory management simulation.
 *
 * This program takes in an input file describing the process that need to be
 * created over the course of the simulation. The file should have once process
 * on every line, in the format "time-created process-id memory-size job-time"
 * where each property is an integer.
 * eg:
 *     0 3 85 30
 *     5 1 100 20
 *     20 6 225 15
 *     24 10 50 14
 *
 * The program also can use one of three algorithms for memory allocation,
 * first fit, best fit and worst fit.
 *
 * Usage: ./swap -f INPUT_FILE -a ALGO -q QUANTUM -m MEMSIZE
 *   INPUT_FILE: file to read processes from
 *         ALGO: one of "first", "best" or "worst"
 *      QUANTUM: quantum for process using cpu
 *      MEMSIZE: the total size of the simulated memory
 *
 */

#include <stdio.h>
#include <assert.h>

#include "argparse.h"
#include "process.h"
#include "proc_creator.h"
#include "linked_list.h"
#include "disk.h"
#include "memory.h"
#include "round_robin_queue.h"

#define TEST_ARGPARSE 0
#define TEST_PROC_CREATOR 0
#define TEST_LINKED_LIST 0
#define TEST_DISK 0
#define TEST_MEMORY 0

#define TEST(title, expected, got) printf("%s: %s: ((expected)%d == (got)%d)\n", ((got) == (expected)) ? "passed" : "FAILED", title, (int)(expected), (int)(got))

#if TEST_ARGPARSE
#include "unit-tests/test_argparse.c"

#elif TEST_PROC_CREATOR
#include "unit-tests/test_proc_creator.c"

#elif TEST_LINKED_LIST
#include "unit-tests/test_linked_list.c"

#elif TEST_DISK
#include "unit-tests/test_disk.c"

#elif TEST_MEMORY
#include "unit-tests/test_memory.c"

#else

/*** Simulation Implementation
 */

int my_ceil(double v) {
    if ((v - (int)v) > 0) {
        return (int)(v + 1);
    } else {
        return (int)v;
    }
}

int main(int argc, char *argv[]) {
    Args args = parse_args(argc, argv);

    // ensure arguments are correct
    assert(args.algorithm != NULL);
    assert(args.filename != NULL);
    assert(args.quantum > 0);
    assert(args.memsize > 0);

    Disk *disk = disk_init();
    Memory *mem = memory_init(args.memsize, args.algorithm);
    ProcCreator *pc = proc_creator_init(args.filename);
    RRQueue *rrqueue = rrqueue_init();
    if (pc == NULL) {
        printf("Unable to create ProcCreator instance.\n");
        return 1;
    }

    Process *cpu_proc = NULL; // Process that currently using the cpu.
    int cpu_quantum = -1; // Quantum left for the process on the cpu.

    int time = 0; // The simulation time

    // Simulation will run while there are more processes to be created
    // or while there is process using the cpu
    while (cpu_proc != NULL || !proc_creator_all_processes_released(pc)) {

        // Load any processes created this cycle onto disk
        int proc_created_this_cycle = 0;
        while (proc_creator_has_next(pc, time)) {
            proc_created_this_cycle = 1;
            disk_swap_in(disk, proc_creator_get_next(pc, time), time);
        }

        // Check if swap/schedule needs to run
        if ((proc_created_this_cycle && memory_is_empty(mem))
                || (cpu_proc && cpu_quantum <= 0)
                || (cpu_proc && cpu_proc->job_time <= 0)) {

            // Remove the process on cpu if it terminated
            if (cpu_proc && cpu_proc->job_time <= 0) {
                memory_swap_out_process(mem, cpu_proc);
                rrqueue_remove(rrqueue, cpu_proc);
                process_destroy(cpu_proc);
                cpu_proc = NULL;
            }

            // Swap section

            // load a process from disk into memory (if any)
            if (!disk_is_empty(disk)) {
                Process *proc_to_load = disk_swap_out(disk);

                rrqueue_push(rrqueue, proc_to_load);

                while (!memory_swap_in(mem, proc_to_load)) {
                    // Unable to swap into memory,
                    // so free space on memory until there is enough.
                    Process *proc = memory_swap_out_oldest(mem);
                    rrqueue_remove(rrqueue, proc);
                    disk_swap_in(disk, proc, time);
                }

                // Also log the load into memory event
                printf("time %d, %d loaded, "
                        "numprocesses=%d, numholes=%d, memusage=%d%%\n",
                    time, proc_to_load->process_id,
                    memory_numprocesses(mem),
                    memory_numholes(mem),
                    my_ceil(memory_memusage(mem)));
            }

            // Schedule section

            // Move the process on the cpu to the back for round robin queue
            // if it's quantum ran out.
            if (cpu_proc && cpu_quantum <= 0) {
                cpu_proc = rrqueue_remove(rrqueue, cpu_proc);
                // note: The process that was on the cpu might no longer be in
                // the rr queue at this point as it may have been swapped out
                // of memory, in which case don't put it back on the queue.
                if (cpu_proc != NULL) {
                    rrqueue_push(rrqueue, cpu_proc);
                }
            }

            // Now let another process use the queue and reset the quantum
            if (!rrqueue_is_empty(rrqueue)) {
                cpu_proc = rrqueue_peek(rrqueue);
                cpu_quantum = args.quantum;
            }
        }

        // End the cycle
        time++;
        if (cpu_proc != NULL) {
            cpu_quantum--;
            cpu_proc->job_time--;
        }
    }

    printf("time %d, simulation finished.\n", (time - 1));

    // successful simulation checks
    assert(proc_creator_all_processes_released(pc));
    assert(disk_is_empty(disk));
    assert(memory_is_empty(mem));
    assert(rrqueue_is_empty(rrqueue));
    assert(NULL == cpu_proc);

    // now free everything
    disk_destroy(disk);
    memory_destroy(mem);
    proc_creator_destroy(pc);
    rrqueue_destroy(rrqueue);

    return 0;
}

#endif
