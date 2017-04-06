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
