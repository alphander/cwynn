#ifndef SHARED_LIBRARY_H
#define SHARED_LIBRARY_H

#include "platform_defines.h"


#ifdef PLATFORM_WINDOWS
#include "windows.h"
typedef HMODULE SharedLibrary;
#elif defined(PLATFORM_UNIX)

typedef void* SharedLibrary;
#endif

SharedLibrary shared_library_open(const char* path);
void shared_library_close(SharedLibrary* pLib);
void* shared_library_load(SharedLibrary* pLib, const char* symbol);

#endif // SHARED_LIBRARY_H