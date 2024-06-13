#ifndef TEST_HEAP_OR_LIST_SORT_H
#define TEST_HEAP_OR_LIST_SORT_H

#include "containers.h"
#include "stdlib.h"
#include "debug/profiling.h"

static int cmp_func2(const void* pDataA, const void* pDataB)
{
    return *(int*)pDataA - *(int*)pDataB;
}

static const size_t randomArraySize = 100000;
static int randomArray[randomArraySize];

void test_heap_or_list_sort()
{
    srand(17);
    for (size_t i = 0; i < randomArraySize; i++)
    {
        randomArray[i] = i;
    }
    
    for (size_t i = 0; i < 1000000; i++)
    {
        size_t indexA = rand() % randomArraySize;
        size_t indexB = rand() % randomArraySize;
        
        int tmp = randomArray[indexA];
        randomArray[indexA] = randomArray[indexB];
        randomArray[indexB] = tmp;
    }

    PROFILER_BEGIN(NULL);
    {
        PROFILER_CAPTUREM("List test start!");

        List list = list_create(sizeof(int));

        for (size_t i = 0; i < randomArraySize; i++)
        {
            list_append(&list, &randomArray[i]);
        }

        list_sort(&list, cmp_func2);
        
        list_destroy(&list);

        PROFILER_CAPTUREM("List test end!");
    }
    {
        PROFILER_CAPTUREM("Heap test start!");
        Heap heap = heap_create(cmp_func2, sizeof(int));

        for (size_t i = 0; i < randomArraySize; i++)
        {
            heap_push(&heap, &randomArray[i]);
        }

        heap_destroy(&heap);

        PROFILER_CAPTUREM("Heap test end!");
    }
    PROFILER_END();
}

#endif // TEST_HEAP_OR_LIST_SORT_H