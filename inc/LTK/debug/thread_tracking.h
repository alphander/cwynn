#ifndef THREAD_TRACKER_H
#define THREAD_TRACKER_H

#include "platform_defines.h"

#ifndef NDEBUG
#include "threading.h"

// By including this header, you override thread_start and thread_wait
// a tracker function that serves the same purpose but also log debug information

// Use THREAD_TRACKER_BEGIN in main function to start the tracking for the whole program
// Use THREAD_TRACKER_END in main function to end the tracking for the whole program
// Use THREAD_TRACKER_REPORT to print currently tracked threadsS

// DO NOT USE DIRECTLY
void thread_tracker_begin();
void thread_tracker_end();
void thread_tracker_thread_start(Thread* pThread, int (*pFunc)(void*), void* pArgs, const char* fileName, const char* funcName, uint32_t lineNumber);
int thread_tracker_thread_wait(Thread* pThread, const char* fileName, const char* funcName, uint32_t lineNumber);
void thread_tracker_report();

// Overrides
#define thread_start(pThread, pFunc, pArgs) thread_tracker_thread_start(pThread, pFunc, pArgs, __FILE__, __func__, __LINE__)
#define thread_wait(pThread) thread_tracker_thread_wait(pThread, __FILE__, __func__, __LINE__)

// Macros
#define THREAD_TRACKER_BEGIN() thread_tracker_begin()
#define THREAD_TRACKER_END() thread_tracker_end()
#define THREAD_TRACKER_REPORT() thread_tracker_report()
#else
#define THREAD_TRACKER_BEGIN() ((void*)0)
#define THREAD_TRACKER_END() ((void*)0)
#define THREAD_TRACKER_REPORT() ((void*)0)
#endif

#endif // THREAD_TRACKER_H