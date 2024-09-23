#ifndef THREADING_H
#define THREADING_H

#include <stdint.h>
#include "platform_defines.h"

// https://stackoverflow.com/questions/18298280/how-to-declare-a-variable-as-thread-local-portably
#ifndef thread_local
# if __STDC_VERSION__ >= 201112 && !defined __STDC_NO_THREADS__
#  define thread_local _Thread_local
# elif defined _WIN32 && (defined _MSC_VER || defined __ICL || defined __DMC__ || defined __BORLANDC__)
#  define thread_local __declspec(thread) 
/* note that ICC (linux) and Clang are covered by __GNUC__ */
# elif defined __GNUC__ || defined __SUNPRO_C || defined __xlC__
#  define thread_local __thread
# else
#  error "Unable to define thread_local"
# endif
#endif


#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <process.h>
typedef uintptr_t Thread;
typedef struct 
{
    INIT_ONCE once;
    CRITICAL_SECTION critical;
} Mutex;
typedef struct
{
    Mutex mutex;
    CONDITION_VARIABLE cond;
} Condition;

#define MUTEX_INIT (Mutex){INIT_ONCE_STATIC_INIT, {0}}
#define CONDITION_INIT (Condition){MUTEX_INIT, 0}

#elif defined(PLATFORM_UNIX)
#include <pthread.h>
typedef pthread_t Thread;
typedef pthread_mutex_t Mutex;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Condition;

#define MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define CONDITION_INIT (Condition){MUTEX_INIT, 0}

#else
#error Threading not supported!
#endif

// ################################################################################
// Thread section
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ################################################################################

/// @brief Runs a task asynchronously using a thread with pFunc as function and pArgs as function arguments
/// @param[in] pFunc Function pointer to thread main function
/// @param[in] pArgs Thread function arguments
void thread_task(int (*pFunc)(void*), void* pArgs);

/// @brief Starts a thread with pFunc as function and pArgs as function arguments
/// @param[in] pFunc Function pointer to thread main function
/// @param[in] pArgs Thread function arguments
/// @return Thread struct
Thread thread_start(int (*pFunc)(void*), void* pArgs);

/// @brief Stops calling thread to wait for pThread
/// @param[in] pThread Pointer to valid Thread handle
/// @return Exit error code
int thread_wait(Thread* pThread);

/// @brief Exits calling thread with error code
/// @param code Error code
void thread_exit(int code);

/// @brief Makes calling thread sleep for specified duration
/// @param milliseconds time length
void thread_sleep(uint64_t milliseconds);

/// @brief Gets current unique thread id
/// @return Calling thread identifier 
uint64_t thread_id();

// ################################################################################
// Mutex Section
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ################################################################################

/// @brief Creates and initializes mutex
/// @return Mutex handle
Mutex mutex_create();

/// @brief Destroys the mutex
/// @param[in] pMutex Pointer to valid Mutex handle
void mutex_destroy(Mutex* pMutex);

/// @brief Locks a mutex, making other mutex_lock calling threads wait or waiting until the mutex is unlocked
/// @param[in] pMutex Pointer to valid Mutex handle
void mutex_lock(Mutex* pMutex);

/// @brief Unlocks a mutex allowing other threads to lock mutex
/// @param[in] pMutex Pointer to valid Mutex handle
void mutex_unlock(Mutex* pMutex);

// ################################################################################
// Condition Section
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ################################################################################

/// @brief Creates a Condition handle
/// @return Condition handle
Condition condition_create();

/// @brief Destroys Condition handle
/// @param[in] pCondition Pointer to valid Condition handle
void condition_destroy(Condition* pCondition);

/// @brief Makes calling thread wait on condition
/// @param[in] pCondition Pointer to valid Condition handle
void condition_wait(Condition* pCondition);

/// @brief Signals the Condition allowing all waiting threads to continue
/// @param[in] pCondition Pointer to valid Condition handle
void condition_signal(Condition* pCondition);

#endif // THREADING_H