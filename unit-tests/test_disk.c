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
