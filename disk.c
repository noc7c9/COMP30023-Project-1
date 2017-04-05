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

#include "linked_list.h"

#include "disk.h"

struct Disk {
    LinkedList *processes;
};

/*
 * The private wrapper struct that is used to store the data about processes on
 * disk.
 */
typedef struct {
    Process *process;
    int store_time;
} _DiskProcess;

Disk *disk_init() {
    Disk *disk = (Disk*)malloc(sizeof(Disk));
    assert(disk);

    disk->processes = linked_list_init();

    return disk;
}

int disk_is_empty(Disk *disk) {
    return linked_list_is_empty(disk->processes);
}

void disk_swap_in(Disk *disk, Process *proc, int time) {
    _DiskProcess *dprocess = (_DiskProcess*)malloc(sizeof(_DiskProcess));
    assert(dprocess);

    dprocess->process = proc;
    dprocess->store_time = time;

    // Newest processes go to the end of the list.
    linked_list_push_end(disk->processes, dprocess);
}

Process *disk_swap_out(Disk *disk) {
    // The processes that have been on the disk the longest are the one(s) at
    // the front of the processes list.

    // So get the process that's at the front of the list.
    Node *node_to_swap = disk->processes->head;

    // Check for other processes that are as old but with a higher priority.
    Node *other_node = node_to_swap->next;
    while (other_node != NULL) {
        _DiskProcess *proc_to_swap = (_DiskProcess*)node_to_swap->data;
        _DiskProcess *other_proc = (_DiskProcess*)other_node->data;

        if (proc_to_swap->store_time < other_proc->store_time) {
            break; // remaining process are younger, so stop
        }

        if (other_proc->process->priority > proc_to_swap->process->priority) {
            // this process has a higher process, so swap this one out instead
            node_to_swap = other_node;
        }

        other_node = other_node->next;
    }

    return ((_DiskProcess*)linked_list_pop(disk->processes, node_to_swap))->process;
}

/*
 * Helper to print out the _DiskProcess instances when printing the whole disk.
 */
void _print_disk_process(void *pointer) {
    _DiskProcess *dprocess = (_DiskProcess*)pointer;
    printf("Disk(%d, ", dprocess->store_time);
    process_print(dprocess->process);
    printf(")\n");
}

void disk_print(Disk *disk) {
    printf("Disk");
    linked_list_print(disk->processes, _print_disk_process);
}
