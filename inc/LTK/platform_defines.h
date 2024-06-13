#ifndef PLATFORM_DEFINES_H
#define PLATFORM_DEFINES_H

// Include this somewhere to catch if a build will fail.

#ifndef __STDC_VERSION__
#error __STDC_VERSION__ wasnt defined
#endif

#if __STDC_VERSION__ < 201112L
#error C version is TOO LOW (change with -std=)
#elif __STDC_VERSION__ > 201710L
#error C version is TOO HIGH (change with -std=)
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_MACOS
#endif

#if defined(__unix__) || defined(__unix)
    #define PLATFORM_UNIX 
#endif

#endif // PLATFORM_DEFINES_H