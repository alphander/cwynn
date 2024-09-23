#ifndef TEST_CONTAINER_LIST_SORT_H
#define TEST_CONTAINER_LIST_SORT_H

#include <stdio.h>
#include "containers.h"

LIST_GENERIC_EX(int, IntList, intlist)

static int container_list_cmp(const int* pValueA, const int* pValueB)
{
    return *pValueA - *pValueB;
}

static inline void container_list_sort()
{
    IntList list = intlist_create();
    intlist_appends(&list, (int[]){10, 2, 65, 23, 28, 2, 84, 3}, 8);
    intlist_sort(&list, container_list_cmp);
    int out;
    while (intlist_iter_next(&list, &out))
    {
        printf("%d\n", out);
    }
}

#endif // TEST_CONTAINER_LIST_SORT_H