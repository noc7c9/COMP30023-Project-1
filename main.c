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
    while (!proc_creator_all_processes_released(pc)) {
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
int main() {
    printf("Testing linked_list\n");

    LinkedList *ll = linked_list_init();
    TEST("initial list empty", 1, linked_list_is_empty(ll));

    int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // push start on empty list
    linked_list_push_start(ll, &v[1]);
    TEST("push_start(_, _) increments len", 1, ll->len);
    TEST("push_start(empty, _) sets head", v[1], *(int *)ll->head->data);
    TEST("push_start(empty, _) sets tail", v[1], *(int *)ll->tail->data);

    // pop start on list with one node
    int popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_start(_, _) decrements len", 0, ll->len);
    TEST("pop_start(_) returns the right element", v[1], popped_value);
    TEST("pop_start(one_element) clears head", 1, NULL == ll->head);
    TEST("pop_start(one_element) clears tail", 1, NULL == ll->tail);

    // push end on empty list
    linked_list_push_end(ll, &v[1]);
    TEST("push_end(_, _) increments len", 1, ll->len);
    TEST("push_end(empty, _) sets head", v[1], *(int *)ll->head->data);
    TEST("push_end(empty, _) sets tail", v[1], *(int *)ll->tail->data);

    // pop end on list with one node
    popped_value = *(int *)linked_list_pop_start(ll);
    TEST("pop_end(_, _) decrements len", 0, ll->len);
    TEST("pop_end(_) returns the right element", v[1], popped_value);
    TEST("pop_end(one_element) clears head", 1, NULL == ll->head);
    TEST("pop_end(one_element) clears tail", 1, NULL == ll->tail);

    // push multiple values
    linked_list_push_start(ll, &v[2]); // list == [v2]
    linked_list_push_end(ll, &v[3]);   // list == [v2, v3]
    linked_list_push_start(ll, &v[1]); // list == [v1, v2, v3]
    TEST("check multiple value 1", v[1], *(int *)ll->head->data);
    TEST("check multiple value 2", v[2], *(int *)ll->head->next->data);
    TEST("check multiple value 3", v[3], *(int *)ll->head->next->next->data);
    TEST("check multiple len is correct", 3, ll->len);

    // insert into the middle
    linked_list_insert_after(ll, ll->head->next, &v[5]); // list == [v1, v2, v5, v3]
    TEST("check post insert value 1", v[1], *(int *)ll->head->data);
    TEST("check post insert value 2", v[2], *(int *)ll->head->next->data);
    TEST("check post insert value 3", v[5], *(int *)ll->tail->prev->data);
    TEST("check post insert value 4", v[3], *(int *)ll->tail->data);

    // removing values from the middle
    linked_list_pop(ll, ll->tail->prev); // list == [v1, v2, v5, v3] => [v1, v2, v3]
    linked_list_pop(ll, ll->head->next); // list == [v1, v2, v3] => [v1, v3]
    TEST("check multiple value post middle pop 1", v[1], *(int *)ll->head->data);
    TEST("check multiple value post middle pop 2", v[3], *(int *)ll->head->next->data);
    TEST("check multiple len is correct post middle pop", 2, ll->len);

    linked_list_insert_after(ll, ll->tail, &v[5]); // list == [v1, v3] => [v1, v3, v5]
    TEST("check inserting after tail", v[5], *(int *)ll->tail->data);
}

#elif TEST_DISK
int main() {
    printf("Testing disk\n");

    Disk *disk = disk_init();
    TEST("initial disk is empty", 1, disk_is_empty(disk));

    disk_swap_in(disk, process_init(-1, 2, -1, -1), 0);
    disk_swap_in(disk, process_init(-1, 1, -1, -1), 0);
    disk_swap_in(disk, process_init(-1, 0, -1, -1), 1);

    // disk_print(disk); printf("\n");

    Process *proc = disk_swap_out(disk);
    TEST("process swapped out check 1", 1, proc->process_id);
    proc = disk_swap_out(disk);
    TEST("process swapped out check 2", 2, proc->process_id);
    proc = disk_swap_out(disk);
    TEST("process swapped out check 3", 0, proc->process_id);

    TEST("disk should now be empty again", 1, disk_is_empty(disk));
}

#elif TEST_MEMORY
int main() {
    printf("Testing memory\n");

    // explicitly define the private structures for testing purposes
    typedef struct {
        int start;
        int size;
        Process *process;
    } Chunk;

    typedef struct {
        Process *process;
        Node *chunk_node;
    } _MemProcess;

    typedef struct {
        LinkedList *chunks;
        LinkedList *processes;
        int size;
    } TestMemory;

    Memory *mem = memory_init(1000, "first");
    TEST("initial memory is empty", 1, memory_is_empty(mem));

    memory_swap_in(mem, process_init(-1, 1, 200, -1));
    memory_swap_in(mem, process_init(-1, 2, 400, -1));
    memory_swap_in(mem, process_init(-1, 3, 300, -1));

    LinkedList *chunks = ((TestMemory*)mem)->chunks;
    Node *node = chunks->head;
    Chunk *chunk = node->data;
    TEST("check memory 1 start", 0, chunk->start);
    TEST("check memory 1 size", 200, chunk->size);
    TEST("check memory 1 process id", 1, chunk->process->process_id);
    node = node->next;
    chunk = node->data;
    TEST("check memory 2 start", 200, chunk->start);
    TEST("check memory 2 size", 400, chunk->size);
    TEST("check memory 2 process id", 2, chunk->process->process_id);
    node = node->next;
    chunk = node->data;
    TEST("check memory 3 start", 600, chunk->start);
    TEST("check memory 3 size", 300, chunk->size);
    TEST("check memory 3 process id", 3, chunk->process->process_id);
    node = node->next;
    chunk = node->data;
    TEST("check memory 4 start", 900, chunk->start);
    TEST("check memory 4 size", 100, chunk->size);
    TEST("check memory 4 is free", 1, NULL == chunk->process);

    memory_swap_out_oldest(mem);
    memory_swap_out_oldest(mem);
    node = chunks->head;
    chunk = node->data;
    TEST("check memory post swap out 1 start", 0, chunk->start);
    TEST("check memory post swap out 1 size", 600, chunk->size);
    TEST("check memory post swap out 1 is free", 1, NULL == chunk->process);
    node = node->next;
    chunk = node->data;
    TEST("check memory post swap out 2 start", 600, chunk->start);
    TEST("check memory post swap out 2 size", 300, chunk->size);
    TEST("check memory post swap out 2 process id", 3, chunk->process->process_id);

    TEST("check memory post swap out len", 3, chunks->len);

    memory_swap_out_process(mem,
            (Process*)((_MemProcess*)((TestMemory*)mem)->processes->head->data)->process);
    node = chunks->head;
    chunk = node->data;
    TEST("check memory post swap out last start", 0, chunk->start);
    TEST("check memory post swap out last size", 1000, chunk->size);
    TEST("check memory post swap out last is free", 1, NULL == chunk->process);
    TEST("check memory post swap out last len", 1, chunks->len);
    TEST("check memory post swap out last non-existent", 1, NULL == node->next);
}

#else
int my_ceil(double v) {
    if ((v - (int)v) > 0) {
        return (int)(v + 1);
    } else {
        return (int)v;
    }
}

int main(int argc, char *argv[]) {
    Args args = parse_args(argc, argv);

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

            // Swap section, load a process from disk into memory (if any)
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

    return 0;
}

#endif
