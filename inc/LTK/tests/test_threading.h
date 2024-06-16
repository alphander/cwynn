#ifndef TEST_THREADING_H
#define TEST_THREADING_H

#include <stdio.h>
#include "threading.h"

static int return_func(void* pArgs)
{
    printf("This is a test!");

    return 0;
}

void test_threading()
{
    Thread thr = thread_start(return_func, NULL);

    thread_wait(&thr);
}

#endif // TEST_THREADING_H