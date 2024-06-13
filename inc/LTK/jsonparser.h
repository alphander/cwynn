#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stdbool.h>
#include "LTK\platform_defines.h"
#include "LTK\error_handling.h"
#include "LTK\containers.h"

#ifndef JSON_KEY_SIZE
#define JSON_KEY_SIZE 256
#endif
#ifndef JSON_POOL_BUFFERS
#define JSON_POOL_BUFFERS 64
#endif

typedef enum
{
    JSON_VALUE_TYPE_NONE,
    JSON_VALUE_TYPE_OBJECT,
    JSON_VALUE_TYPE_ARRAY,
    JSON_VALUE_TYPE_STRING,
    JSON_VALUE_TYPE_NUMBER,
    JSON_VALUE_TYPE_BOOL,
    JSON_VALUE_TYPE_NULL,
} JsonValueType;

typedef struct
{
    char str[JSON_KEY_SIZE];
} JsonKey;

typedef struct JsonValue JsonValue;

DICT_GENERIC_EX(JsonKey, JsonValue*, JsonDict, jsondict)
LIST_GENERIC_EX(JsonValue*, JsonList, jsonlist)

struct JsonValue
{
    JsonValueType valueType;
    union
    {
        JsonDict object;
        JsonList array;
        char* string;
        double number;
        bool boolean;
    };
};

typedef struct
{
    JsonValue* valueBuffers[JSON_POOL_BUFFERS];
    size_t valueBufferUtilizationArray[JSON_POOL_BUFFERS];
    int valueBufferIndex;

    char* stringBuffers[JSON_POOL_BUFFERS];
    size_t stringBufferUtilizationArray[JSON_POOL_BUFFERS];
    int stringBufferIndex;

    bool init;
} JsonPool;

// ################################################################################
// Token finding
//
//
//
// ################################################################################

/// @brief Returns a JsonPool that handles JsonValue and string memory 
//  (Any memory allocated will stay allocated until the pool is destroyed. There is no garbage collection)
/// @return JsonPool struct
JsonPool json_pool_create();

/// @brief Allocates a JsonValue
/// @param[in] pPool Pointer to valid JsonPool struct
/// @return Pointer to JsonValue in pool
JsonValue* json_value_alloc(JsonPool* pPool);

/// @brief Allocates enough memory for the requested string size
/// @param[in] pPool Pointer to valid JsonPool struct
/// @param stringSize Size of string in bytes to allocate
/// @return Pointer to string in pool
char* json_string_alloc(JsonPool* pPool, size_t stringSize);

/// @brief Destroys json pool and cleans all JsonValues and json strings
/// @param[in] pPool Pointer to valid JsonPool struct
void json_pool_destroy(JsonPool* pPool);

/// @brief Parses a json string into a JsonValue tree
/// @param[in] pPool Pointer to valid JsonPool struct
/// @param[in] jsonString Json string
/// @return JsonValue tree root node
JsonValue* json_parse(JsonPool* pPool, char* jsonString);

/// @brief Takes a JsonValue tree and converts it to a json string
/// @param[in] pPool Pointer to valid JsonPool struct
/// @param[in] pJsonValue JsonValue node
/// @param[out] pSizeOut Size of string memory 
/// @return Json string handled by JsonPool
const char* json_serialize(JsonPool* pPool, JsonValue* pJsonValue, size_t* pSizeOut);

/// @brief Attempts to return a string type
/// @param pJsonValue JsonValue node
/// @return constant string
const char* json_to_string(JsonValue* pJsonValue);

/// @brief Attempts to return a number type
/// @param pJsonValue JsonValue node
/// @return double
double json_to_number(JsonValue* pJsonValue);

/// @brief Attempts to return a bool type
/// @param pJsonValue JsonValue node
/// @return bool
bool json_to_bool(JsonValue* pJsonValue);

/// @brief Attempts to return a JsonDict* type
/// @param pJsonValue JsonValue node
/// @return JsonDict dict generic pointer
JsonDict* json_to_object(JsonValue* pJsonValue);

/// @brief Attempts to return a JsonList* type
/// @param pJsonValue JsonValue node
/// @return JsonList list generic pointer
JsonList* json_to_array(JsonValue* pJsonValue);

/// @brief Attempts to index into an array type
/// @param pJsonValue JsonValue node
/// @param index Index into the internal List
/// @return Sub JsonValue node
JsonValue* json_array_get(JsonValue* pJsonValue, int64_t index);

/// @brief Attempts to get from an object type
/// @param pJsonValue JsonValue node
/// @param pKey Key to get entry in internal Dict
/// @return Sub JsonValue node
JsonValue* json_object_get(JsonValue* pJsonValue, JsonKey key);

/// @brief Attempts to iterate with an array type
/// @param pJsonValue JsonValue node
/// @return Sub JsonValue node
JsonValue* json_array_next(JsonValue* pJsonValue);

/// @brief Attempts to iterate with an array type
/// @param pJsonValue JsonValue node
/// @param pKeyOut Optional out parameter for Key. CAN be NULL
/// @return Sub JsonValue node
JsonValue* json_object_next(JsonValue* pJsonValue, JsonKey* pKeyOut);

/// @brief // Static inline function for verifying value types
/// @param pJsonValue JsonValue node
/// @param jsonType JsonValueType enum
/// @return Comparison outcome
static inline bool json_is_type(JsonValue* pJsonValue, JsonValueType jsonType)
{
    return pJsonValue->valueType == jsonType;
}

/// @brief Utility print function
/// @param pJsonValue Valid Pointer to JsonValue
void json_print_tree(JsonValue* pJsonValue);

#endif // JSONPARSER_H