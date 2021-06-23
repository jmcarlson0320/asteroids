#include "list.h"
#include <stdlib.h>



Node *node_new(void *val)
{
    Node *new = malloc(sizeof(Node));
    new->val = val;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

struct List {
    Node *head;
    Node *tail;
    int length;
};

List *list_new()
{
    List *new = malloc(sizeof(List));
    new->head = NULL;
    new->tail = NULL;
    new->length = 0;
    return new;
}

void list_delete(List *list)
{
    if (!list) {
        return;
    }
    Node *cur = list->head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(list);
}

void list_append(List *list, void *val)
{
    if (!list || !val) {
        return;
    }

    Node *new = node_new(val);
    if (list->tail) {
        list->tail->next = new;
        new->prev = list->tail;
    } else {
        list->head = new;
    }
    list->tail = new;
    list->length++;
}

void list_remove(List *list, void *val)
{
    if (!list || !val) {
        return;
    }

    int found = 0;
    Node *cur = list->head;
    while (cur && !found) {
        if (cur->val == val) {
            found = 1;
            break;
        }
        cur = cur->next;
    }

    if (found) {
        if (!cur->prev && !cur->next) { // single node
            list->head = NULL;
            list->tail = NULL;
        } else if (!cur->prev) { // head
            cur->next->prev = NULL;
            list->head = cur->next;
        } else if (cur->next) { // body
            cur->next->prev = cur->prev;
            cur->prev->next = cur->next;
        } else { // tail
            cur->prev->next = NULL;
            list->tail = cur->prev;
        }
        free(cur);
        list->length--;
    }
}

void *list_pop(List *list, int pos)
{
    if (!list || pos < 0 || list_len(list) <= pos) {
        return NULL;
    }

    int index = 0;
    Node *cur = list->head;
    while (cur && index < pos) {
        cur = cur->next;
        index++;
    }

    if (!cur->prev && !cur->next) { // single node
        list->head = NULL;
        list->tail = NULL;
    } else if (!cur->prev) { // head
        cur->next->prev = NULL;
        list->head = cur->next;
    } else if (cur->next) { // body
        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
    } else { // tail
        cur->prev->next = NULL;
        list->tail = cur->prev;
    }
    void *val = cur->val;
    free(cur);
    list->length--;

    return val;
}

int list_len(List *list)
{
    return list->length;
}

List_Iterator list_iterator(List *list)
{
    List_Iterator it = {
        .cur = list->head
    };

    return it;
}

void *list_next(List_Iterator *it)
{
    void *item = NULL;
    if (it->cur) {
        item = it->cur->val;
        it->cur = it->cur->next;
    }
    return item;
}

int list_has_next(List_Iterator *it)
{
    return it->cur != NULL;
}
