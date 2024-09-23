#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "LTK\platform_defines.h"

// Breaking convention and making a typedef enum lowercase (Errval -> errval).
// We expect this to be returned and intend to have it be visually silent for the programmer.
typedef enum
{
    ERR_SUCCESS = 0,
    ERR_FAILURE = 1,
    ERR_CHECK_FAILED,
    ERR_PANIC_FAILED,
    ERR_INVALID_ARGS,
    ERR_IO_FAILED,
    ERR_OBJECT_NOT_INIT,
    ERR_OUT_OF_MEMORY,
    ERR_PARSING,
    ERR_SHARED_LIBRARY_OPENING,
    ERR_SHARED_LIBRARY_LOADING,
} errval;

struct errinfo
{
    const char* funcName;
    const char* fileName;
    uint32_t lineNumber;
};

/// @brief Registers and sets the error callback (If Null, will return to default callback)
/// @param[in] pErrorCallback Error callback function 
void err_callback_register(void (*pErrorCallback)(errval err, struct errinfo info, const char* errMsg));

/// @brief The default error callback. Can be called by a custom registered callback.
/// @param err error value
/// @param info additional info about err stored in a struct errinfo
/// @param errMsg Additional custom message
void err_default_callback(errval err, struct errinfo info, const char* errMsg);

/// @brief Calls the currently set error callback
/// @param err Error type
/// @param info Debug info for file and location details of the error
/// @param[in] errMsg Extra string info for the callback
/// @return Value passed in by err
errval err_call(errval err, struct errinfo info, const char* errMsg);

/// @brief Returns the name of the errval passed in as a string e.g. (ERRVAL_FAILURE) -> "ERRVAL_FAILURE"
/// @param err Error type
/// @return errval enum to string name e.g. (ERRVAL_INVALID_ARGS) -> "ERRVAL_INVALID_ARGS"
const char* err_name(errval err);

/// @brief Gets last global errval
/// @return last errval
errval err_get();

/// @brief Sets internal errval
/// @return last errval
errval err_set(errval err);

#ifndef NERR_CALLBACK
// Call an error
#define ERR_CALL(err) err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, "")
#define ERR_CALLM(err, msg) err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, msg)
// Call and return with ret
#define ERR_RET(exp, err, ret) {if (exp) {err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, "("#exp")"); return ret;}}
#define ERR_RETM(exp, err, ret, msg) {if (exp) {err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, "("#exp") \""msg"\""); return ret;}}
// Call and return err value
#define ERR_RETV(exp, err) {if (exp) {err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, "("#exp")"); return err;}}
#define ERR_RETVM(exp, err, msg) {if (exp) {err_call(err, (struct errinfo){__func__, __FILE__, __LINE__}, "("#exp") \""msg"\""); return err;}}
// Check and or exit
#define ERR_CHECK(err) {if (err) {err_call(ERR_CHECK_FAILED, (struct errinfo){__func__, __FILE__, __LINE__}, "ERR_CHECK guard failed!"); exit(EXIT_FAILURE);}}
#define ERR_PANIC() {if (err_get()) {err_call(ERR_PANIC_FAILED, (struct errinfo){__func__, __FILE__, __LINE__}, "ERR_PANIC guard failed!"); exit(EXIT_FAILURE);}}

#else
#define ERR_CALL(err) err_set(err)
#define ERR_CALLM(err, msg) err_set(err)
#define ERR_RET(exp, err, ret) {if (exp) {err_set(err); return ret;}}
#define ERR_RETM(exp, err, ret, msg) {if (exp) {err_set(err); return ret;}}
#define ERR_RETV(exp, err) {if (exp) {err_set(err); return ret;}}
#define ERR_RETVM(exp, err, msg) {if (exp) {err_set(err); return ret;}}
#define ERR_CHECK(err) {if (err) {err_set(ERR_CHECK_FAILED); exit(EXIT_FAILURE);}}
#define ERR_PANIC() if (err_get()) {err_set(ERR_PANIC_FAILED); exit(EXIT_FAILURE);}
#endif

#ifndef NDEBUG
#define ERR_PRINT(fstring, ...) fprintf(stderr, fstring, __VA_ARGS__)
#else
#define ERR_PRINT(fstring, ...) ((void*)0)
#endif

#endif // ERROR_HANDLING_H