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

Node *_new_node();

LinkedList *linked_list_init() {
    LinkedList *ll = (LinkedList*)malloc(sizeof(LinkedList));
    assert(ll);

    ll->head = NULL;
    ll->tail = NULL;
    ll->len = 0;

    return ll;
}

int linked_list_is_empty(LinkedList *ll) {
    return ll->len == 0;
}

void linked_list_push_start(LinkedList *ll, void *data) {
    Node *node = _new_node();
    node->data = data;

    if (ll->head) { // point to previous head (if any)
        node->next = ll->head;
        ll->head->prev = node;
    }
    ll->head = node; // set as the new head

    // also set as tail if is the only element in the list
    if (ll->tail == NULL) {
        ll->tail = node;
    }

    ll->len++;
}

void linked_list_push_end(LinkedList *ll, void *data) {
    Node *node = _new_node();
    node->data = data;

    if (ll->tail) { // point to previous tail (if any)
        node->prev = ll->tail;
        ll->tail->next = node;
    }
    ll->tail = node; // set as the new tail

    // also set as head if is the only element in the list
    if (ll->head == NULL) {
        ll->head = node;
    }

    ll->len++;
}

void *linked_list_pop_start(LinkedList *ll) {
    Node *node = ll->head;
    void *data = node->data;

    // fix head pointer
    ll->head = node->next;

    // also fix the tail pointer if necessary
    if (node->next == NULL) {
        ll->tail = NULL;
    }

    free(node);

    ll->len--;

    return data;
}

void *linked_list_pop_end(LinkedList *ll) {
    Node *node = ll->tail;
    void *data = node->data;

    // fix tail pointer
    ll->tail = node->prev;

    // also fix the head pointer if necessary
    if (node->prev == NULL) {
        ll->head = NULL;
    }

    free(node);

    ll->len--;

    return data;
}

void linked_list_print(LinkedList *ll, void (*print_node)(void*)) {
    printf("List(\n");
    Node *n = ll->head;
    while (n) {
        printf("   ");
        if (print_node != NULL) {
            print_node(n->data);
        } else {
            // just print the void* pointer if no print_node func is given
            printf("%p\n", n->data);
        }
        n = n->next;
    }
    printf(")");
}

/*
 * Simple helper to allocate a new node
 */
Node *_new_node() {
    Node *n = (Node*)malloc(sizeof(Node));
    assert(n);

    n->next = NULL;
    n->prev = NULL;
    n->data = NULL;

    return n;
}