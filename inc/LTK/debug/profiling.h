#ifndef PROFILING_H
#define PROFILING_H

#include "platform_defines.h"

#ifndef NDEBUG
#include <stddef.h> 
#include <stdint.h>

// Use PROFILER_BEGIN() to start the profiler (This must be called before and PROFILER_CAPTURE() gets called!)
// Use PROFILER_CAPTURE() to capture a frame of data
// Use PROFILER_END() to stop profiler

// DO NOT USE DIRECTLY
void profiler_begin(const char* reportWriteDir);
void profiler_end();
void profiler_capture(const char* message, const char* fileName, const char* funcName, uint32_t lineNumber);
void profiler_report();

// Begins profiler
#define PROFILER_BEGIN(reportWriteDir) {profiler_begin(reportWriteDir); profiler_capture("...Init sample...", __FILE__, __func__, __LINE__);}
#define PROFILER_CAPTUREM(message) profiler_capture(message, __FILE__, __func__, __LINE__)
#define PROFILER_CAPTURE() profiler_capture(NULL, __FILE__, __func__, __LINE__)
#define PROFILER_END() profiler_end()
#define PROFILER_REPORT() profiler_report()
#define PROFILER_WRITE() profiler_write(PROFILER_LOG_PATH)
#else
#define PROFILER_BEGIN(fileName) ((void*)0)
#define PROFILER_CAPTUREM(message) ((void*)0)
#define PROFILER_CAPTURE() ((void*)0)
#define PROFILER_END() ((void*)0)
#define PROFILER_REPORT() ((void*)0)
#define PROFILER_WRITE() ((void*)0)
#endif
#endif // PROFILING_H