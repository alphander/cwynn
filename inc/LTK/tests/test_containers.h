#include "containers.h"
#include "error_handling.h"
#include "test.h"

typedef struct
{
    uint8_t bytes[143];
} RandomStruct;

// We first prove that a function will work in all cases.
// Only then can it be used in proving that other functions function in all case.
// For example if we prove that list_create works, we can use list_create to prove that
// list_appends works

// create:
// destroy:
// is_init:
// clear:
// append:
// appends:
// remove:
// removes:
// set:
// get:
// iter_next:
// iter_reset:
// indexof:
// contains:
// reserve:
// reserved:
// size:
// type_size:
// to_array:

static void test_list_create()
{
    {
        List list = list_create(0);

        assert(list.capacity == 0);
        assert(list.iterIndex == 0);
        assert(list.pMemory == NULL);
        assert(list.reserved == 0);
        assert(list.size == 0);
        assert(list.stride == 0);
    }
    {
        size_t typeSize = rand_typesize();

        List list = list_create(typeSize);
        assert(list.capacity == 8);
        assert(list.iterIndex == 0);
        assert(list.pMemory != NULL);
        assert(list.reserved == 8);
        assert(list.size == 0);
        assert(list.stride == typeSize);

        free(list.pMemory);
    }
}

static void test_list_destroy()
{
    {
        list_destroy(NULL);
        // Address sanitizer and error logger
    }
    {
        size_t typeSize = rand_typesize();

        List list = list_create(typeSize);
        list_destroy(&list);
        // Address sanitizer and error logger
        assert(list.pMemory == NULL);
    }
}

static void test_list_is_init()
{
    {
        assert(!list_is_init(NULL));
    }
    {
        List list = {0};
        assert(!list_is_init(&list));
    }
    {
        size_t typeSize = rand_typesize();

        List list = list_create(typeSize);
        assert(list_is_init(&list));
        list_destroy(&list);
        assert(!list_is_init(&list));
    }
}

static void test_list_append()
{
    {
        list_append(NULL, NULL);
    }
    {
        size_t typeSize = rand_typesize();
        RANDOM_ARRAY(arr, typeSize);

        list_append(NULL, arr);
    }
    {
        size_t typeSize = rand_typesize();
        List list = list_create(typeSize);

        list_append(&list, NULL);

        list_destroy(&list);
    }
    {
        size_t typeSize = rand_typesize();
        List list = list_create(typeSize);

        size_t fillSize = rand_range(1, 100);
        RANDOM_ARRAY(array, typeSize * fillSize);

        for (size_t i = 0; i < rand_range(1, 531); i++)
        {
            RANDOM_ARRAY(array, typeSize);
            list_append(&list, array);
        }

        list_destroy(&list);
    }
}

void containers_list_test()
{
    test_list_create();
    test_list_destroy();
    test_list_is_init();
    test_list_append();
}

void containers_list_test_old()
{
    size_t typeSize = rand_typesize();

    List list = {0};
    assert(!list_is_init(&list));
    list = list_create(typeSize);
    assert(list_is_init(&list));

    RANDOM_ARRAY(a, typeSize);
    RANDOM_ARRAY(b, typeSize);
    RANDOM_ARRAY(c, typeSize);

    list_append(&list, a);
    list_append(&list, b);
    list_append(&list, c);

    size_t fillLevel = rand() % 234;
    RANDOM_ARRAY(t, typeSize * fillLevel)

    list_appends(&list, t, fillLevel);
    
    uint8_t aOut[typeSize];
    uint8_t bOut[typeSize];
    uint8_t cOut[typeSize];
    list_get(&list, 0, aOut);
    list_get(&list, 1, bOut);
    list_get(&list, 2, cOut);

    uint8_t tOut[typeSize * fillLevel];
    for (size_t i = 0; i < fillLevel; i++)
        list_get(&list, i + 3, &tOut[typeSize * i]);

    assert(!memcmp(a, aOut, typeSize));
    assert(!memcmp(b, bOut, typeSize));
    assert(!memcmp(c, cOut, typeSize));
    assert(!memcmp(t, tOut, typeSize * fillLevel));
    assert(list_size(&list) == fillLevel + 3);
    
    list_remove(&list, 0);
    assert(list_size(&list) == fillLevel + 2);

    uint8_t dOut[typeSize];
    uint8_t eOut[typeSize];
    list_get(&list, 0, dOut);
    list_get(&list, 1, eOut);
    assert(!memcmp(b, dOut, typeSize));
    assert(!memcmp(c, eOut, typeSize));

    while (list_size(&list) > 0)
    {
        int64_t removeSpot = rand() % list_size(&list);
        int64_t removeLength = list_size(&list) - removeSpot;
        list_removes(&list, removeSpot, removeLength);
    }

    assert(list_size(&list) == 0);

    size_t fillArrayLength = rand() % 412 + 1;
    uint8_t fillArray[typeSize * fillArrayLength];
    for (size_t i = 0; i < fillArrayLength; i++)
        fillArray[i * typeSize] = ((uint8_t)i) | 0x01;
    list_appends(&list, fillArray, fillArrayLength);

    uint8_t empty[typeSize];
    memset(empty, 0, typeSize);

    size_t searchValue = rand() % fillArrayLength;
    list_set(&list, searchValue, empty);

    list_contains(&list, empty);

    int64_t searchedIndex = list_indexof(&list, empty);
    assert(searchedIndex == searchValue);

    list_clear(&list);
    assert(list_size(&list) == 0);

    list_destroy(&list);
}

void containers_queue_test()
{

}

void containers_dict_test()
{

}

void containers_set_test()
{

}

void containers_pool_test()
{

}