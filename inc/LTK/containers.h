#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <stdint.h>
#include <stdbool.h>

// ################################################################################
// List Section
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

typedef struct
{
    int64_t iterIndex;
    size_t stride;
    int64_t size;
    int64_t capacity;
    int64_t reserved;
    uint8_t* pMemory;
} List;

/// @brief Creates a List object
/// @param stride Size of data type
/// @return List struct
List list_create(size_t stride);

#define LIST_CREATE(type) list_create(sizeof(type))

/// @brief Destroys a List object
/// @param[in] pList Pointer to valid List struct
void list_destroy(List* pList);

/// @brief Checks if List is initialized with list_create
/// @param[in] pList Pointer to valid List struct
/// @return TRUE if initialized and FALSE if not
bool list_is_init(List* pList);

/// @brief Clears all data in list
/// @param[in] pList Pointer to valid List struct
void list_clear(List* pList);

/// @brief Appends data to the end of the List
/// @param[in] pList Pointer to valid List struct
/// @param[in] pData Pointer to data to be copied
void list_append(List* pList, void* pData);

/// @brief Appends count amount of elements to the end of the list 
/// @param[in] pList Pointer to valid List struct
/// @param[in] pData Pointer to data to be copied
/// @param count Number of elements to copy
void list_appends(List* pList, void* pData, size_t count);

/// @brief Removes data from the List at the requested index
/// @param[in] pList Pointer to valid List struct
/// @param index Index of the element to remove
void list_remove(List* pList, int64_t index);

/// @brief Removes a section data from the List at the requested index
/// @param[in] pList Pointer to valid List struct
/// @param index Index of the first element to remove
/// @param count Number of elements to remove
void list_removes(List* pList, int64_t index, size_t count);

/// @brief Sets data in the List at the requested index
/// @param[in] pList Pointer to valid List struct
/// @param index Array index to set
/// @param[in] pData Pointer to data to be copied
void list_set(List* pList, int64_t index, void* pData);

/// @brief Gets data from the List at the requested index
/// @param[in] pList Pointer to valid List struct
/// @param index Index of the element to get from
/// @param[out] pDataOut Pointer to location to copy to
void list_get(List* pList, int64_t index, void* pDataOut);

/// @brief Gets next entry in the List based on an iterator
/// @param[in] pSet Pointer to valid List struct
/// @param[out] pDataOut Returns the data
/// @return TRUE if iterator has a next value, FALSE if none
bool list_iter_next(List* pList, void* pDataOut);

/// @brief Resets the set iterator index
/// @param[in] pSet Pointer to valid List struct
void list_iter_reset(List* pList);

/// @brief Does a linear search through the List and returns the index of the first occurance of pData
/// @param[in] pList Pointer to valid List struct
/// @param[in] pData Pointer to data to search for
/// @return Element present in the List, -1 if not present
int64_t list_indexof(List* pList, void* pData);

/// @brief Does a linear search to determine if List contains pData
/// @param[in] pList Pointer to valid List struct
/// @param[in] pData Pointer to data to search for
/// @return TRUE if element is present in the List, FALSE if not
bool list_contains(List* pList, void* pData);

/// @brief Sorts array based on a comparison function
/// @param[in] pList Pointer to valid List struct
/// @param[in] cmpFunc Comparison function based on qsort
void list_sort(List* pList, int (*cmpFunc)(const void* pDataA, const void* pDataB));

/// @brief Reserves a number of minumum elements
/// @param[in] pList Pointer to valid List struct
/// @param reserveSize Amount of elements to reserve
void list_reserve(List* pList, size_t reserveSize);

/// @brief Gets the reserved number of minumum elements in the List
/// @param[in] pList Pointer to valid List struct
/// @return Number of minumum elements of the List
size_t list_reserved(List* pList);

/// @brief Gets the number of elements in the List
/// @param[in] pList Pointer to valid List struct
/// @return Size of the List
size_t list_size(List* pList);

/// @brief Gets the array type size (a.k.a. stride) of the List
/// @param[in] pList Pointer to valid List struct
/// @return Type size of the List
size_t list_type_size(List* pList);

/// @brief Copies to pMemoryOut the array of elements inside the list
/// @param[in] pList Pointer to valid List struct
/// @param[out] pMemoryOut Pointer to valid memory of length (stride * size)
void list_to_array(List* pList, void* pMemoryOut);

// ################################################################################
// Queue Section
//
// ## For the sake of avoiding superfluity, we've chosen the name "queue"
// ## instead of "deque" as there won't be other type for "deque"s or "stack"s in this package.
// 
// ## For example, one justification comes from the name "double ended queue"
// ## or "deque" for short. This signifies that it is a type of queue with the
// ## property of being double ended.
// ## It is specified because it needs to be distiguished from other types of 
// ## data structures of the same family.
//
// ## In our case however, for the sake of clarity in such a small package with very
// ## few data structures, we made the decision to forgo the use of 
// ## specific type names and use the more abstract simple terms they refer to.
//
//
// ################################################################################

typedef struct
{
    int64_t iterIndex;
    size_t stride;
    int64_t size;
    int64_t capacity;
    int64_t reserved;
    int64_t backIndex;
    int64_t frontIndex;
    uint8_t* pMemory;
} Queue;

/// @brief Creates and fills out Queue structure
/// @param stride Size of type
/// @return Queue struct
Queue queue_create(size_t stride);

#define QUEUE_CREATE(type) queue_create(sizeof(type))

/// @brief Destroys Queue and frees internal memory
/// @param[in] pDict Pointer to valid Dict struct
void queue_destroy(Queue* pQueue);

/// @brief Checks if Queue is initialized with dueue_create
/// @param[in] pQueue Pointer to valid Queue struct
/// @return TRUE if initialized and FALSE if not
bool queue_is_init(Queue* pQueue);

/// @brief Pushes data to the front of the queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[in] pData Pointer to data to be copied
void queue_push_front(Queue* pQueue, void* pData);

/// @brief Pushes data to the back of the queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[in] pData Pointer to data to be copied
void queue_push_back(Queue* pQueue, void* pData);

/// @brief Copies data from the front of the queue to pData
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[out] pDataOut Pointer to data to be copied to
void queue_peek_front(Queue* pQueue, void* pDataOut);

/// @brief Copies data from the back of the queue to pData
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[out] pDataOut Pointer to data to be copied to
void queue_peek_back(Queue* pQueue, void* pDataOut);

/// @brief Removes data at the front of the queue and (optionally) returns it
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[out] pDataOut Pointer to data to be copied to (NULLABLE)
void queue_pop_front(Queue* pQueue, void* pDataOut);

/// @brief Removes data at the back of the queue and (optionally) returns it
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[out] pDataOut Pointer to data to be copied to (NULLABLE)
void queue_pop_back(Queue* pQueue, void* pDataOut);

/// @brief Gets value at index in queue starting from backIndex
/// @param[in] pQueue Pointer to valid Queue struct
/// @param index Index into the queue, starting from the back to the front
/// @param[out] pDataOut Pointer to data to be copied to
void queue_get(Queue* pQueue, int64_t index, void* pDataOut);

/// @brief Gets next entry in the Queue based on an iterator
/// @param[in] pSet Pointer to valid Queue struct
/// @param[out] pDataOut Returns the data
/// @return TRUE if iterator has a next value, FALSE if none
bool queue_iter_next(Queue* pQueue, void* pDataOut);

/// @brief Resets the set iterator index
/// @param[in] pSet Pointer to valid Queue struct
void queue_iter_reset(Queue* pQueue);

/// @brief Does a linear search through the Queue and returns the index of the first occurance of pData
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[in] pData Pointer to data to search for
/// @return Element present in the Queue, -1 if not present
int64_t queue_indexof(Queue* pQueue, void* pData);

/// @brief Does a linear search to determine if Queue contains pData
/// @param[in] pList Pointer to valid Queue struct
/// @param[in] pData Pointer to data to search for
/// @return TRUE if element is present in the Queue, FALSE if not
bool queue_contains(Queue* pQueue, void* pData);

/// @brief Reserves a minimum capacity for the queue internal memory
/// @param[in] pQueue Pointer to valid Queue struct
/// @param reserveSize Amount of elements to reserve
void queue_reserve(Queue* pQueue, size_t reserveSize);

/// @brief Gets the reserved number of minumum elements in the Queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @return Number of minumum elements of the Queue
size_t queue_reserved(Queue* pQueue);

/// @brief Gets the number of elements in the Queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @return Size of the Queue
size_t queue_size(Queue* pQueue);

/// @brief Gets the array stride of the Queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @return Type size of the Queue
size_t queue_stride(Queue* pQueue);

/// @brief Copies to pMemoryOut the array of elements inside the queue
/// @param[in] pQueue Pointer to valid Queue struct
/// @param[out] pMemoryOut Pointer to valid memory of length (stride * size)
void queue_to_array(Queue* pQueue, void* pMemoryOut);

// ################################################################################
// Heap Section
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

typedef struct
{
    int (*cmpFunc)(const void* pDataA, const void* pDataB);
    size_t size;
    size_t stride;
    size_t capacity;
    uint8_t* pMemory;
} Heap;

Heap heap_create(int (*cmpFunc)(const void* pDataA, const void* pDataB), size_t stride);

void heap_destroy(Heap* pHeap);

bool heap_is_init(Heap* pHeap);

void heap_push(Heap* pHeap, void* pData);

bool heap_pop(Heap* pHeap, void* pDataOut);

void heap_peek(Heap* pHeap, void* pDataOut);

size_t heap_size(Heap* pHeap);

size_t heap_stride(Heap* pHeap);

// ################################################################################
// Dict Section
//
// ## We've chosen the term "Dict" to refer to a dictionary. Other appropriate
// ## words we could have chosen are "Map", "HashMap" and "HashTable".
//
//
//
//
//
//
//
//
// ################################################################################

typedef struct
{
    int64_t iterIndex;
    // Amount of space that data type occupies
    size_t stateStride;
    size_t keyStride;
    size_t valueStride;
    size_t entryStride;

    // Offset is the byte position in the entry that the data type starts at
    int64_t stateOffset;
    int64_t keyOffset;
    int64_t valueOffset;
    
    int64_t size;
    int64_t capacity;
    uint8_t* pMemory;
} Dict;

/// @brief Creates and fills out Dict structure
/// @param keySize Size of key type
/// @param valueSize Size of value type
/// @return Dict struct
Dict dict_create(size_t keySize, size_t valueSize);

#define DICT_CREATE(keyType, valueType) dict_create(sizeof(keyType), sizeof(valueType))

/// @brief Destroys Dict and frees internal Memory
/// @param[in] pDict Pointer to valid Dict struct
void dict_destroy(Dict* pDict);

/// @brief Checks if Dict is initialized with dict_create
/// @param[in] pDict Pointer to valid Dict struct
/// @return TRUE if initialized and FALSE if not
bool dict_is_init(Dict* pDict);

/// @brief Inserts Key and value pair into the Dict
/// @param[in] pDict Pointer to valid Dict struct
/// @param[in] pKey Pointer to Key data to be copied
/// @param[in] pValue Pointer to value data to be copied
void dict_put(Dict* pDict, void* pKey, void* pValue);

/// @brief Remove entry from Dict at Key
/// @param[in] pDict Pointer to valid Dict struct
/// @param[in] pKey Pointer to Key data to be copied
void dict_remove(Dict* pDict, void* pKey);

/// @brief Retrieves value from Dict at Key
/// @param[in] pDict Pointer to valid Dict struct
/// @param[in] pKey Pointer to Key data to be copied
/// @param[out] pValueOut Pointer to location to copy to
void dict_get(Dict* pDict, void* pKey, void* pValueOut);

/// @brief Searches Dict for Key
/// @param[in] pDict Pointer to valid Dict struct
/// @return TRUE if contains Key
bool dict_contains(Dict* pDict, void* pKey);

/// @brief Gets next entry in the Dict based on an iterator (At least one must NOT be NULL)
/// @param[in] pDict Pointer to valid Dict struct
/// @param[out] pKeyOut Returns the entry key (NULLABLE)
/// @param[out] pValueOut Returns the entry value (NULLABLE)
/// @return TRUE if iterator has a next value, FALSE if none
bool dict_iter_next(Dict* pDict, void* pKeyOut, void* pValueOut);

/// @brief Resets the dict iterator index
/// @param[in] pDict Pointer to valid Dict struct
void dict_iter_reset(Dict* pDict);

/// @brief Gets the number of entries in the Dict
/// @param[in] pDict Pointer to valid Dict struct
/// @return Number of entries in the Dict
size_t dict_size(Dict* pDict);

/// @brief Gets the size of the Key type
/// @param[in] pDict Pointer to valid Dict struct
/// @return Key size of the Dict
size_t dict_key_size(Dict* pDict);

/// @brief Gets the size of the Value type
/// @param[in] pDict Pointer to valid Dict struct
/// @return Value size of the Dict
size_t dict_value_size(Dict* pDict);

/// @brief Copies to pValuesOut the array of elements inside the queue
/// @param[in] pDict Pointer to valid Dict struct
/// @param[out] pValuesOut Pointer to valid memory of length (valueStride * size)
void dict_values_to_array(Dict* pDict, void* pValuesOut);

/// @brief Copies to pKeysOut the array of elements inside the queue
/// @param[in] pDict Pointer to valid Dict struct
/// @param[out] pKeysOut Pointer to valid memory of length (keyStride * size)
void dict_keys_to_array(Dict* pDict, void* pKeysOut);

// ################################################################################
// Set Section
//
// ## This Set is completely built on top of a Dict
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

typedef struct
{
    Dict dict;
} Set;

/// @brief Creates a Set object
/// @param stride Size of data type
/// @return Set struct
Set set_create(size_t stride);

#define SET_CREATE(type) set_create(sizeof(type))

/// @brief Destroys Set and frees internal Memory
/// @param[in] pSet Pointer to valid Set struct
void set_destroy(Set* pSet);

/// @brief Checks if Set is initialized with set_create
/// @param[in] pSet Pointer to valid Set struct
/// @return TRUE if initialized and FALSE if not
bool set_is_init(Set* pSet);

/// @brief Puts data in the Set
/// @param[in] pSet Pointer to valid Set struct
/// @param[in] pData Pointer to data to be copied
void set_put(Set* pSet, void* pData);

/// @brief Remove element from Set
/// @param[in] pSet Pointer to valid Set struct
/// @param[in] pData Pointer to data to be copied
void set_remove(Set* pSet, void* pData);

/// @brief Searches Set for pData
/// @param[in] pSet Pointer to valid Set struct
/// @return TRUE if contains Data
bool set_contains(Set* pSet, void* pData);

/// @brief Gets next entry in the Set based on an iterator
/// @param[in] pSet Pointer to valid Set struct
/// @param[out] pDataOut Returns the data
/// @return TRUE if iterator has a next value, FALSE if none
bool set_iter_next(Set* pSet, void* pDataOut);

/// @brief Resets the set iterator index
/// @param[in] pSet Pointer to valid Set struct
void set_iter_reset(Set* pSet);

/// @brief Gets the type size (a.k.a. stride) of the Set
/// @param[in] pSet Pointer to valid Set struct
/// @return Type size of the Set
size_t set_type_size(Set* pSet);

/// @brief Gets the number of entries in the Set
/// @param[in] pSet Pointer to valid Set struct
/// @return Number of entries in the Set
size_t set_size(Set* pSet);

/// @brief Copies to pDataOut the array of elements inside the Set
/// @param[in] pSet Pointer to valid Dict struct
/// @param[out] pDataOut Pointer to valid memory of length (type_size * size)
void set_to_array(Set* pSet, void* pDataOut);

// ################################################################################
// Pool Section
//
// ## This is an object pool. 
// ## It keeps the memory necessary for objects and keeps track of allocation
//
// ## Unlike other data structures, 
// ## this returns pointers into the memory of the pool instead of copies.
// ## This means that realloc cannot be used to grow the memory.
//
//
//
//
//
//
//
//
// ################################################################################

#define POOL_BUFFERS 128

typedef struct
{
    uint8_t* buffers[POOL_BUFFERS];
    size_t bufferCount;
    Queue unused;
    size_t stride;
    size_t size;
    size_t capacity;
} Pool;

/// @brief Creates a Pool object
/// @param stride Size of data type
/// @return Pool struct
Pool pool_create(size_t stride);

#define POOL_CREATE(type) pool_create(sizeof(type))

/// @brief Destroys Pool and frees internal Memory
/// @param[in] pPool Pointer to valid Pool struct
void pool_destroy(Pool* pPool);

/// @brief Checks if Pool is initialized with pool_create
/// @param[in] pPool Pointer to valid Pool struct
/// @return TRUE if initialized and FALSE if not
bool pool_is_init(Pool* pPool);

/// @brief Allocates internal memory and returns pointer to a buffer of size stride
/// @param[in] pPool Pointer to valid Pool struct
/// @return pointer to a buffer of size stride held by the Pool
void* pool_alloc(Pool* pPool);

/// @brief Returns memory to Pool that was previously allocated from pool_alloc
/// @param[in] pPool Pointer to valid Pool struct
/// @param[in] pObject Pointer to object data
void pool_return(Pool* pPool, void* pObject);

/// @brief Number of Objects held by the Pool
/// @param[in] pPool Pointer to valid Pool struct
/// @return Used object count
size_t pool_size(Pool* pPool);

/// @brief Gets the type size (a.k.a. stride) of the Pool
/// @param[in] pPool Pointer to valid Pool struct
/// @return pool object size
size_t pool_type_size(Pool* pPool);

// ################################################################################
// Generic Code
//
//
// Creates a wrapper around code with macros that replace void pointers with types
// and makes a unique type and function for each type you call ####_GENERIC(type) for
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

// ################################################################################
// List Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_LIST_GENERIC_PASTE(T, TS, TF)\
typedef struct {List list;} TS;\
\
static inline TS TF##_create() {\
    return (TS){list_create(sizeof(T))};\
}\
static inline void TF##_destroy(TS* pList) {\
    list_destroy(&pList->list);\
}\
static inline bool TF##_is_init(TS* pList) {\
    return list_is_init(&pList->list);\
}\
static inline void TF##_clear(TS* pList) {\
    list_clear(&pList->list);\
}\
static inline void TF##_append(TS* pList, T data) {\
    list_append(&pList->list, &data);\
}\
static inline void TF##_appends(TS* pList, T* pData, size_t count) {\
    list_appends(&pList->list, pData, count);\
}\
static inline void TF##_remove(TS* pList, int64_t index) {\
    list_remove(&pList->list, index);\
}\
static inline void TF##_removes(TS* pList, int64_t index, size_t count) {\
    list_removes(&pList->list, index, count);\
}\
static inline void TF##_set(TS* pList, int64_t index, T data) {\
    list_set(&pList->list, index, &data);\
}\
static inline T TF##_get(TS* pList, int64_t index) {\
    T tmp;\
    list_get(&pList->list, index, &tmp);\
    return tmp;\
}\
static inline bool TF##_iter_next(TS* pList, T* pDataOut) {\
    return list_iter_next(&pList->list, pDataOut);\
}\
static inline void TF##_iter_reset(TS* pList) {\
    list_iter_reset(&pList->list);\
}\
static inline int64_t TF##_indexof(TS* pList, T data) {\
    return list_indexof(&pList->list, &data);\
}\
static inline bool TF##_contains(TS* pList, T data) {\
    return list_contains(&pList->list, &data);\
}\
static inline void TF##_sort(TS* pList, int (*cmpFunc)(const T* pDataA, const T* pDataB)) {\
    list_sort(&pList->list, (int (*)(const void*, const void*))cmpFunc);/*might be causing undefined behavior*/\
}\
static inline void TF##_reserve(TS* pList, size_t reserveSize) {\
    list_reserve(&pList->list, reserveSize);\
}\
static inline size_t TF##_reserved(TS* pList) {\
    return list_reserved(&pList->list);\
}\
static inline size_t TF##_size(TS* pList) {\
    return list_size(&pList->list);\
}\
static inline void TF##_to_array(TS* pList, T* pMemoryOut) {\
    list_to_array(&pList->list, pMemoryOut);\
}\

#define LIST_GENERIC_EX(T, TS, TF) CONTAINERS_LIST_GENERIC_PASTE(T, TS, TF)
#define LIST_GENERIC(T) LIST_GENERIC_EX(T, List_## T, list_## T)

// ################################################################################
// Queue Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_QUEUE_GENERIC_PASTE(T, TS, TF)\
typedef struct {Queue queue;} TS;\
\
static inline TS TF##_create() {\
    return (TS){queue_create(sizeof(T))};\
}\
static inline void TF##_destroy(TS* pQueue) {\
    queue_destroy(&pQueue->queue);\
}\
static inline bool TF##_is_init(TS* pQueue) {\
    return queue_is_init(&pQueue->queue);\
}\
static inline void TF##_push_front(TS* pQueue, T data) {\
    queue_push_front(&pQueue->queue, &data);\
}\
static inline void TF##_push_back(TS* pQueue, T data) {\
    queue_push_back(&pQueue->queue, &data);\
}\
static inline T TF##_peek_front(TS* pQueue) {\
    T tmp = {0};\
    queue_peek_front(&pQueue->queue, &tmp);\
    return T;\
}\
static inline T TF##_peek_back(TS* pQueue) {\
    T tmp = {0};\
    queue_peek_back(&pQueue->queue, &tmp);\
    return T;\
}\
static inline T TF##_pop_front(TS* pQueue) {\
    T tmp = {0};\
    queue_pop_front(&pQueue->queue, &tmp);\
    return T;\
}\
static inline T TF##_pop_back(TS* pQueue) {\
    T tmp = {0};\
    queue_pop_back(&pQueue->queue, &tmp);\
    return T;\
}\
static inline T TF##_get(TS* pQueue, int64_t index) {\
    return queue_get(&pQueue->queue, index);\
}\
static inline bool TF##_iter_next(TS* pQueue, T* pDataOut) {\
    return queue_iter_next(&pQueue->queue, &pDataOut);\
}\
static inline void TF##_iter_reset(TS* pQueue) {\
    queue_iter_reset(&pQueue->queue);\
}\
static inline int64_t TF##_indexof(TS* pQueue, T data) {\
    return queue_indexof(&pQueue->queue, &data);\
}\
static inline bool TF##_contains(TS* pQueue, T data) {\
    return queue_contains(&pQueue->queue, &data);\
}\
static inline void TF##_reserve(TS* pQueue, size_t reserveSize) {\
    queue_reserve(&pQueue->queue, reserveSize);\
}\
static inline size_t TF##_reserved(TS* pQueue) {\
    return queue_reserved(&pQueue->queue);\
}\
static inline size_t TF##_size(TS* pQueue) {\
    return queue_size(&pQueue->queue);\
}\
static inline void TF##_to_array(TS* pQueue, T* pMemoryOut) {\
    return queue_to_array(&pQueue->queue, pMemoryOut);\
}\

#define QUEUE_GENERIC_EX(T, TS, TF) CONTAINERS_QUEUE_GENERIC_PASTE(T, TS, TF)
#define QUEUE_GENERIC(T) QUEUE_GENERIC_EX(T, Queue_## T, queue_## T)

// ################################################################################
// Heap Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_HEAP_GENERIC_PASTE(T, TS, TF)\
typedef struct {Heap heap;} TS;\
\
static inline TS TF##_create(int (*cmpFunc)(const T* pDataA, const T* pDataB)) {\
    return (TS){heap_create((int (*)(const void*, const void*))cmpFunc, sizeof(T))};/*might be causing undefined behavior*/\
}\
static inline void TF##_destroy(TS* pHeap) {\
    heap_destroy(&pHeap->heap);\
}\
static inline bool TF##_is_init(TS* pHeap) {\
    return heap_is_init(&pHeap->heap);\
}\
static inline void TF##_push(TS* pHeap, T data) {\
    heap_push(&pHeap->heap, &data);\
}\
static inline T TF##_pop(TS* pHeap) {\
    T tmp = {0};\
    heap_pop(&pHeap->heap, &tmp);\
    return tmp;\
}\
static inline T TF##_peek(TS* pHeap) {\
    T tmp = {0};\
    heap_peek(&pHeap->heap, &tmp);\
    return tmp;\
}\
static inline size_t TF##_size(TS* pHeap) {\
    return heap_size(&pHeap->heap);\
}\

#define HEAP_GENERIC_EX(T, TS, TF) CONTAINERS_HEAP_GENERIC_PASTE(T, TS, TF)
#define HEAP_GENERIC(T) HEAP_GENERIC_EX(T, Queue_## T, queue_## T)

// ################################################################################
// Dict Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_DICT_GENERIC_PASTE(K, V, TS, TF)\
typedef struct {Dict dict;} TS;\
\
static inline TS TF##_create() {\
    return (TS){dict_create(sizeof(K), sizeof(V))};\
}\
static inline void TF##_destroy(TS* pDict) {\
    dict_destroy(&pDict->dict);\
}\
static inline bool TF##_is_init(TS* pDict) {\
    return dict_is_init(&pDict->dict);\
}\
static inline void TF##_put(TS* pDict, K key, V value) {\
    dict_put(&pDict->dict, &key, &value);\
}\
static inline void TF##_remove(TS* pDict, K key) {\
    dict_remove(&pDict->dict, &key);\
}\
static inline V TF##_get(TS* pDict, K key) {\
    V tmp = {0};\
    dict_get(&pDict->dict, &key, &tmp);\
    return tmp;\
}\
static inline bool TF##_contains(TS* pDict, K key) {\
    return dict_contains(&pDict->dict, &key);\
}\
static inline bool TF##_iter_next(TS* pDict, K* pKeyOut, V* pValueOut) {\
    return dict_iter_next(&pDict->dict, pKeyOut, pValueOut);\
}\
static inline void TF##_iter_reset(TS* pDict) {\
    return dict_iter_reset(&pDict->dict);\
}\
static inline size_t TF##_size(TS* pDict) {\
    return dict_size(&pDict->dict);\
}\
static inline void TF##_values_to_array(TS* pDict, V* pValuesOut) {\
    dict_values_to_array(&pDict->dict, pValuesOut);\
}\
static inline void TF##_keys_to_array(TS* pDict, K* pKeysOut) {\
    dict_values_to_array(&pDict->dict, pKeysOut);\
}\

#define DICT_GENERIC_EX(K, V, TS, TF) CONTAINERS_DICT_GENERIC_PASTE(K, V, TS, TF)
#define DICT_GENERIC(K, V) DICT_GENERIC_EX(K, V, Dict_## K ##_## V, dict_## K ##_## V)

// ################################################################################
// Set Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_SET_GENERIC_PASTE(T, TS, TF)\
typedef struct {Set set;} TS;\
\
static inline TS TF##_create() {\
    return (TS){set_create(sizeof(T))};\
}\
static inline void TF##_destroy(TS* pSet) {\
    set_destroy(&pSet->set);\
}\
static inline bool TF##_is_init(TS* pSet) {\
    return set_is_init(&pSet->set);\
}\
static inline void TF##_put(TS* pSet, T data) {\
    set_put(&pSet->set, &data);\
}\
static inline void TF##_remove(TS* pSet, T data) {\
    set_remove(&pSet->set, &data);\
}\
static inline bool TF##_contains(TS* pSet, T data) {\
    return set_contains(&pSet->set, &data);\
}\
static inline bool TF##_iter_next(TS* pSet, T* pDataOut) {\
    return set_iter_next(&pSet->set, pDataOut);\
}\
static inline void TF##_iter_reset(TS* pSet) {\
    return set_iter_reset(&pSet->set);\
}\
static inline size_t TF##_size(TS* pSet) {\
    return set_size(&pSet->set);\
}\
static inline void TF##_to_array(TS* pSet, T* pDataOut) {\
    set_to_array(&pSet->set, pDataOut);\
}\

#define SET_GENERIC_EX(T, TS, TF) CONTAINERS_SET_GENERIC_PASTE(T, TS, TF)
#define SET_GENERIC(T) SET_GENERIC_EX(T, Set_## T, set_## T)

// ################################################################################
// Pool Generic Section
//
//
//
// ################################################################################

#define CONTAINERS_POOL_GENERIC_PASTE(T, TS, TF)\
typedef struct {Pool pool;} TS;\
\
static inline TS TF##_create() {\
    return (TS){pool_create(sizeof(T))};\
}\
static inline void TF##_destroy(TS* pPool) {\
    pool_destroy(&pPool->pool);\
}\
static inline bool TF##_is_init(TS* pPool) {\
    return pool_is_init(&pPool->pool);\
}\
static inline T* TF##_alloc(TS* pPool) {\
    return pool_alloc(&pPool->pool);\
}\
static inline void TF##_return(TS* pPool, T* pObject) {\
    pool_return(&pPool->pool, pObject);\
}\
static inline size_t TF##_size(TS* pPool) {\
    return pool_size(&pPool->pool);\
}\

#define POOL_GENERIC_EX(T, TS, TF) CONTAINERS_POOL_GENERIC_PASTE(T, TS, TF)
#define POOL_GENERIC(T) POOL_GENERIC_EX(T, Pool_## T, pool_## T)

#endif // CONTAINERS_H