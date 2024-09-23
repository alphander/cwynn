#ifndef TEST_CONTAINER_DICT_H
#define TEST_CONTAINER_DICT_H

#include "containers.h"

DICT_GENERIC_EX(int, int, IntDict, intdict);

static void container_dict()
{
    IntDict d = intdict_create();

    intdict_put(&d, 10, 2);
    intdict_put(&d, 10, 7);
    intdict_put(&d, 15, 9);
    intdict_put(&d, 12, 0);

    int keyOut;
    int valueOut;
    while (intdict_iter_next(&d, &keyOut, &valueOut))
    {
        printf("key: %d value: %d\n", keyOut, valueOut);
    }

    intdict_destroy(&d);
}

#endif // TEST_CONTAINER_DICT_H