#ifndef TEST_CONTAINER_HEAP_H
#define TEST_CONTAINER_HEAP_H
#include "LTK\containers.h"

static int cmp_func(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

void container_heap()
{
    Heap heap = heap_create(cmp_func, sizeof(int));
    heap_push(&heap, &(int){0});
    heap_push(&heap, &(int){7});
    heap_push(&heap, &(int){8});
    heap_push(&heap, &(int){3});
    heap_push(&heap, &(int){9});
    heap_push(&heap, &(int){4});
    heap_push(&heap, &(int){2});
    size_t heapSize = heap_size(&heap);
    for (size_t i = 0; i < heapSize; i++)
    {
        int out = 0;
        heap_pop(&heap, &out);
        printf("%d\n", out);
    }

    heap_destroy(&heap);
}

#endif // TEST_CONTAINER_HEAP_H