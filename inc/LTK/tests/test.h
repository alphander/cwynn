#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define RANDOM_ARRAY(array, typeSize) uint8_t array[typeSize]; fill_random(array, typeSize);

static inline size_t rand_range(size_t from, size_t to)
{
    assert(from < to);
    
    return from + rand() % (to - from);
}

static void fill_random(uint8_t array[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        array[i] = rand() % UINT8_MAX;
    }
}

static inline size_t rand_typesize()
{
    return rand_range(1, 300);
}

#endif // TEST_H