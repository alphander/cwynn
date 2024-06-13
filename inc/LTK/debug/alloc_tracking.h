#ifndef ALLOC_TRACKER_H
#define ALLOC_TRACKER_H

#include "LTK\platform_defines.h"

#ifndef NDEBUG
#include <stdint.h>

// By including this header, you override malloc, calloc, realloc and free with 
// a tracker function that serves the same purpose but also log debug information

// Use ALLOC_TRACKER_BEGIN in main function to start the tracking for the whole program
// Use ALLOC_TRACKER_END in main function to end the tracking for the whole program
// Use ALLOC_TRACKER_REPORT to print currently tracked memory allocations

// DO NOT USE DIRECTLY
void* alloc_tracker_malloc(size_t size, const char* fileName, const char* funcName, uint32_t lineNumber);
void* alloc_tracker_calloc(size_t num, size_t size, const char* fileName, const char* funcName, uint32_t lineNumber);
void* alloc_tracker_realloc(void* pMemory, size_t size, const char* fileName, const char* funcName, uint32_t lineNumber);
void alloc_tracker_free(void* pMemory);
void alloc_tracker_begin();
void alloc_tracker_end();
void alloc_tracker_report();

// Overrides
#define malloc(size) alloc_tracker_malloc(size, __FILE__, __func__, __LINE__)
#define calloc(num, size) alloc_tracker_calloc(num, size, __FILE__, __func__, __LINE__)
#define realloc(pMemory, size) alloc_tracker_realloc(pMemory, size, __FILE__, __func__, __LINE__)
#define free(pMemory) alloc_tracker_free(pMemory)

// Macros
#define ALLOC_TRACKER_BEGIN() alloc_tracker_begin()
#define ALLOC_TRACKER_END() alloc_tracker_end()
#define ALLOC_TRACKER_REPORT() alloc_tracker_report()
#else
#define ALLOC_TRACKER_BEGIN() ((void*) 0)
#define ALLOC_TRACKER_END() ((void*) 0)
#define ALLOC_TRACKER_REPORT() ((void*) 0)
#endif

#endif // ALLOC_TRACKER_H