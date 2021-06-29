#include "list.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *a = "joshua";
    char *b = "pieper";

    List *l = list_new();

    list_insert(l, 0, a);
    list_insert(l, 0, b);
    

    List_Iterator it = list_iterator(l);
    while (list_has_next(&it)) {
        printf("%s\n", (char *) list_next(&it));
    }
    return 0;
}
