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

#include "linked_list.h"

#include "memory.h"

struct Memory {
    LinkedList *chunks;
    LinkedList *processes;
    int size;
    Node *(*algorithm)(Memory*, int);
};

/*
 * A private struct used to represent individual chunks of memory.
 */
typedef struct {
    int start;
    int size;
    Process *process; // the process using this chunk of memory (NULL if not in use)
} Chunk;

/*
 * The private wrapper struct that is used to store the data about processes on
 * memory.
 */
typedef struct {
    Process *process;
    Node *chunk_node; // the node that stores the chunk of memory that this
                      // process is loaded into
} _MemProcess;

Chunk *_new_chunk(int start, int size, Process *process);

void _merge_adjecent(Memory *mem, Node* chunk_node);

Process *_remove_mem_process(Memory *mem, _MemProcess *mprocess);

Node *_first_fit(Memory *mem, int size);
Node *_best_fit(Memory *mem, int size);
Node *_worst_fit(Memory *mem, int size);

Memory *memory_init(int size, char *algorithm) {
    Memory *mem = (Memory*)malloc(sizeof(Memory));
    assert(mem);

    mem->processes = linked_list_init();
    mem->chunks = linked_list_init();
    mem->size = size;

    // for simplicity's sake just check the first character
    switch (algorithm[0]) {
        case 'f':
            mem->algorithm = _first_fit;
            break;
        case 'b':
            mem->algorithm = _best_fit;
            break;
        case 'w':
            mem->algorithm = _worst_fit;
            break;
    }

    // initialize chunks list with a single chunk representing the whole memory
    linked_list_push_start(mem->chunks, _new_chunk(0, size, NULL));

    return mem;
}

int memory_is_empty(Memory *mem) {
    return linked_list_is_empty(mem->processes);
}

int memory_swap_in(Memory *mem, Process *proc) {
    Node *node = mem->algorithm(mem, proc->memory_size);

    // if no valid chunk was found, return false
    if (node == NULL) {
        return 0;
    }

    // a valid chunk was found
    Chunk *chunk = node->data;

    // first add the process to the list of processes
    _MemProcess *mprocess = (_MemProcess*)malloc(sizeof(_MemProcess));
    assert(mprocess);

    mprocess->process = proc;
    mprocess->chunk_node = NULL; // will be set later in the function

    // newest process goes on the end
    linked_list_push_end(mem->processes, mprocess);

    // if the chunk is exactly the required size, reuse it
    if (chunk->size == proc->memory_size) {
        chunk->process = proc;
        mprocess->chunk_node = node;

        return 1; // return success
    }

    // otherwise split it into two new chunks

    // add a new chunk as in use by the proc
    // (inserted after the old chunk)
    Node *used = linked_list_insert_after(mem->chunks, node,
            _new_chunk(chunk->start, proc->memory_size, proc));
    mprocess->chunk_node = used;

    // add another chunk for the remaining free memory
    // (inserted after the new used chunk)
    linked_list_insert_after(mem->chunks, used,
            _new_chunk(chunk->start + proc->memory_size,
                chunk->size - proc->memory_size, NULL));

    // then remove the old chunk
    linked_list_pop(mem->chunks, node);

    return 1; // return success
}

Process *memory_swap_out_process(Memory *mem, Process *proc) {
    // search for the given process
    Node *node = mem->processes->head;
    while (node) {
        if (proc == ((_MemProcess*)node->data)->process) {
            // found!

            // now remove it
            return _remove_mem_process(mem,
                    (_MemProcess*)linked_list_pop(mem->processes, node));
        }
        node = node->next;
    }

    return proc;
}

Process *memory_swap_out_oldest(Memory *mem) {
    // oldest process is at the front
    _MemProcess *mprocess = linked_list_pop_start(mem->processes);

    return _remove_mem_process(mem, mprocess);
}

int memory_numprocesses(Memory *mem) {
    Node *node = mem->processes->head;
    int n = 0;
    while (node) {
        n++;
        node = node->next;
    }
    return n;
}

int memory_numholes(Memory *mem) {
    Node *node = mem->chunks->head;
    int n = 0;
    while (node) {
        n += (((Chunk*)node->data)->process == NULL);
        node = node->next;
    }
    return n;
}

double memory_memusage(Memory *mem) {
    Node *node = mem->chunks->head;
    int used = 0;
    while (node) {
        Process *proc = ((Chunk*)node->data)->process;
        used += (proc == NULL) ? 0 : proc->memory_size;
        node = node->next;
    }
    return 100.0 * used / mem->size;
}

/*
 * Helper to print out the Chunk instances when printing the memory.
 */
void _print_chunk(void *pointer) {
    Chunk *chunk = (Chunk*)pointer;
    printf("MemoryChunk(start=%d, size=%d, process_id=%d)\n",
            chunk->start, chunk->size,
            (chunk->process == NULL) ? -1 : chunk->process->process_id);
}

/*
 * Helper to print out the _MemProcess instances when printing the memory.
 */
void _print_mem_process(void *pointer) {
    _MemProcess *mprocess = (_MemProcess*)pointer;
    printf("MemoryProcess(");
    process_print(mprocess->process);
    printf("\n      using ");
    _print_chunk(mprocess->chunk_node->data);
    printf("   )\n");
}

void memory_print(Memory *mem) {
    printf("MemoryChunks");
    linked_list_print(mem->chunks, _print_chunk);
    printf("\nMemoryProcesses");
    linked_list_print(mem->processes, _print_mem_process);
}

/*
 * Helper to allocate a new chunk.
 */
Chunk *_new_chunk(int start, int size, Process *process) {
    Chunk *chunk = (Chunk*)malloc(sizeof(Chunk));
    assert(chunk);

    chunk->start = start;
    chunk->size = size;
    chunk->process = process;

    return chunk;
}

/*
 * Helper that removes a _MemProcess instance as well as mark as free its
 * corresponding Chunk.
 */
Process *_remove_mem_process(Memory *mem, _MemProcess *mprocess) {
    Process *proc = mprocess->process;

    // mark chunk used by the process as free
    ((Chunk *)mprocess->chunk_node->data)->process = NULL;
    _merge_adjecent(mem, mprocess->chunk_node);

    free(mprocess);

    return proc;
}

/*
 * Helper to merge adjecent memory chunks into one.
 */
void _merge_adjecent(Memory *mem, Node* chunk_node) {
    // look for neighboring nodes that are also free
    Node *first;
    Node *last;
    first = last = chunk_node;
    while (first->prev && ((Chunk*)first->prev->data)->process == NULL) {
        first = first->prev;
    }
    while (last->next && ((Chunk*)last->next->data)->process == NULL) {
        last = last->next;
    }

    // if there are multiple adjecent free nodes, merge them
    if (first != last) {
        Chunk *first_chunk = first->data;
        Chunk *last_chunk = last->data;

        // reuse the first node as the merged node
        first_chunk->size = last_chunk->start + last_chunk->size - first_chunk->start;

        // remove all other nodes
        while (last != first) {
            last = last->prev;
            linked_list_pop(mem->chunks, last->next);
        }
    }
}

/*
 * The first fit memory placement implementation.
 * Finds the first free chunk thats large enough from the list of chunks.
 * Returns the node of that chunk.
 */
Node *_first_fit(Memory *mem, int required_size) {
    Node *node = mem->chunks->head;
    while (node != NULL) {
        Chunk *chunk = (Chunk *)node->data;
        if (chunk->process == NULL && chunk->size >= required_size) {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

/*
 * The best fit memory placement implementation.
 * Finds the smallest free chunk thats large enough from the list of chunks.
 * Returns the node of that chunk.
 */
Node *_best_fit(Memory *mem, int required_size) {
    Node *smallest = NULL;

    Node *node = mem->chunks->head;
    while (node != NULL) {
        Chunk *chunk = (Chunk *)node->data;
        if (chunk->process == NULL && chunk->size >= required_size) {
            if (smallest == NULL
                    || chunk->size < ((Chunk *)smallest->data)->size) {
                smallest = node;
            }
        }

        node = node->next;
    }

    return smallest;
}

/*
 * The worst fit memory placement implementation.
 * Finds the biggest free chunk thats large enough from the list of chunks.
 * Returns the node of that chunk.
 */
Node *_worst_fit(Memory *mem, int required_size) {
    Node *biggest = NULL;

    Node *node = mem->chunks->head;
    while (node != NULL) {
        Chunk *chunk = (Chunk *)node->data;
        if (chunk->process == NULL && chunk->size >= required_size) {
            if (biggest == NULL
                    || chunk->size > ((Chunk *)biggest->data)->size) {
                biggest = node;
            }
        }

        node = node->next;
    }

    return biggest;
}
