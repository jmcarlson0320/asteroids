#ifndef LIST_H
#define LIST_H

typedef struct List List;
typedef struct Node Node;
typedef struct List_Iterator List_Iterator;

struct List {
    Node *head;
    Node *tail;
    int length;
};

struct Node {
    void *val;
    Node *next;
    Node *prev;
};

struct List_Iterator {
    Node *cur;
};

List *list_new();
void list_delete(List *list);
void list_append(List *list, void *val);
void list_remove(List *list, void *val);
void *list_pop(List *list, int pos);
int list_len(List *list);

List_Iterator list_iterator(List *list);
void *list_next(List_Iterator *it);
int list_has_next(List_Iterator *it);

/* implement as needed
void list_clear(List *list);
void list_insert(List *list, int pos, void *val);
void *list_get(List *list, int pos);
int list_contains(List *list, void *val);
int list_index(List *list, void *val);
*/

#endif // LIST_H
