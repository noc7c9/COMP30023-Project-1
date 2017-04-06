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
