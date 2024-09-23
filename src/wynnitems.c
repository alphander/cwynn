#include "wynnitems.h"
#include "float.h"
#include <math.h>
#include <LTK/threading.h>

static float sliderValues[WYNNITEM_ID_ARRAY_SIZE] = {0};
static Mutex sliderValuesMutex = MUTEX_INIT;

static WynnItemIdArray mins = {0};
static WynnItemIdArray maxs = {0};
#define SORTED_ITEMS_COUNT 8
static WynnItemList sortedItems[SORTED_ITEMS_COUNT] = {0};
// 0 == helmets
// 1 == chestplates
// 2 == leggsings
// 3 == boots
// 4 == rings
// 5 == bracelets
// 6 == necklaces
// 7 == weapons
#define BUILD_SIZE 9 // Rings twice

static inline float lerp(int32_t a, int32_t b, float t)
{
    return b + (b - a) * t;
}

static inline float distanceSqrd(int32_t a, int32_t b)
{
    return ((float)b - a) * ((float)b - a);
}

void wynnitems_init(WynnItemList* pItemList)
{
    for (size_t i = 0; i < SORTED_ITEMS_COUNT; i++)
    {
        sortedItems[i] = wynnitem_list_create();
    }

    for (size_t i = 0; i < WYNNITEM_ID_ARRAY_SIZE; ++i)
    {
        maxs[i] = INT32_MIN;
        mins[i] = INT32_MAX;
    }

    WynnItem* pItem = NULL;
    while (wynnitem_list_iter_next(pItemList, &pItem))
    {
        for (size_t i = 0; i < WYNNITEM_ID_ARRAY_SIZE; ++i)
        {
            if (mins[i] > pItem->idArray[i])
                mins[i] = pItem->idArray[i]; 
            if (maxs[i] < pItem->idArray[i])
                maxs[i] = pItem->idArray[i];
        }    
    }

    size_t count = wynnitem_list_size(pItemList);
    for (size_t i = 0; i < count; i++)
    {
        WynnItem* pItem = wynnitem_list_get(pItemList, i);
        switch(pItem->type)
        {
            case WYNNITEM_TYPE_HELMET: wynnitem_list_append(&sortedItems[0], pItem); break;
            case WYNNITEM_TYPE_CHESTPLATE: wynnitem_list_append(&sortedItems[1], pItem); break;
            case WYNNITEM_TYPE_LEGGINGS: wynnitem_list_append(&sortedItems[2], pItem); break;
            case WYNNITEM_TYPE_BOOTS: wynnitem_list_append(&sortedItems[3], pItem); break;
            case WYNNITEM_TYPE_RING: wynnitem_list_append(&sortedItems[4], pItem); break;
            case WYNNITEM_TYPE_BRACELET: wynnitem_list_append(&sortedItems[5], pItem); break;
            case WYNNITEM_TYPE_NECKLACE: wynnitem_list_append(&sortedItems[6], pItem); break;
            case WYNNITEM_TYPE_WEAPON: wynnitem_list_append(&sortedItems[7], pItem); break;
        }
    }
}

void wynnitems_cleanup()
{
    for (size_t i = 0; i < SORTED_ITEMS_COUNT; i++)
    {
        wynnitem_list_destroy(&sortedItems[i]);
    }
}

float wynnitem_get_value(size_t index)
{
    mutex_lock(&sliderValuesMutex);
    float value = sliderValues[index];
    mutex_unlock(&sliderValuesMutex);

    return value;
}

void wynnitem_set_value(size_t index, float value)
{
    mutex_lock(&sliderValuesMutex);
    sliderValues[index] = value;
    mutex_unlock(&sliderValuesMutex);
}

float wynnitem_similarity(WynnItem* pItem, WynnItem* pTestItem)
{
    float v = 0;
    for (size_t i = 0; i < WYNNITEM_ID_ARRAY_SIZE; i++)
    {
        v += distanceSqrd(pItem->idArray[i], pTestItem->idArray[i]);
    }

    return sqrtf(v); // Remove in future
}

static float item_target_distance(WynnItem* pItem, float* pTargets)
{
    float v = 0;
    for (size_t i = 0; i < WYNNITEM_ID_ARRAY_SIZE; ++i)
    {
        float a = pItem->idArray[i];
        float b = pTargets[i];
        v += (b - a) * (b - a);
    }
    return v;
}

static float evaluate_build(WynnBuild* pBuild, float* pTargets)
{
    float accum = 0.f;
    for (size_t i = 0; i < 9; i++)
    {
        accum += item_target_distance(pBuild->pItems[i], pTargets);
    }
    return accum;
}

WynnBuild wynnitems_calculate_build(size_t numIters)
{
    // srand(10);

    WynnBuild build = {0};

    float targets[WYNNITEM_ID_ARRAY_SIZE];
    for (size_t i = 0; i < WYNNITEM_ID_ARRAY_SIZE; i++)
    {
        targets[i] = lerp(mins[i], maxs[i], sliderValues[i] + .5f);
    }

    size_t indices[] = {0, 1, 2, 3, 4, 4, 5, 6, 7};
    for (size_t i = 0; i < 9; ++i)
    {
        WynnItemList* pList = &sortedItems[indices[i]];
        size_t randomIndex = rand() % wynnitem_list_size(pList);
        build.pItems[i] = wynnitem_list_get(pList, randomIndex);
    }

    float lowestScore = evaluate_build(&build, targets);
    for (size_t iter = 0, i = 0; iter < numIters; ++iter, i = iter % 9)
    {
        WynnBuild buildCopy = build;
        WynnItemList* pList = &sortedItems[indices[i]];
        size_t randomIndex = rand() % wynnitem_list_size(pList);
        buildCopy.pItems[i] = wynnitem_list_get(pList, randomIndex);
        float score = evaluate_build(&buildCopy, targets);
        if (score < lowestScore) continue;
        
        lowestScore = score;
        build = buildCopy;
    }

    return build;
};

// Edit one piece at a time to see if build improves and also start at different configurations
//  to descend the gradient at different locations hoping to find different local minima.
// Find solutions to the rucksack problem (numberphile)

// 93,973,101,503,386,604,499,000 Iterations required for brute force
// WynnBuild wynnitems_calculate_build()
// {
//     float targetValues[WYNNITEM_ID_ARRAY_SIZE] = {0}; 

//     size_t bestIndices[9] = {0};
//     float closestScore = FLT_MAX;

//     size_t indices[9] = {0};
//     for (indices[0] = 0; indices[0] < wynnitem_list_size(&sortedItems[0]); ++indices[0])
//     for (indices[1] = 0; indices[1] < wynnitem_list_size(&sortedItems[1]); ++indices[1])
//     for (indices[2] = 0; indices[2] < wynnitem_list_size(&sortedItems[2]); ++indices[2])
//     for (indices[3] = 0; indices[3] < wynnitem_list_size(&sortedItems[3]); ++indices[3])
//     for (indices[4] = 0; indices[4] < wynnitem_list_size(&sortedItems[4]); ++indices[4]) // [4] == rings
//     for (indices[5] = 0; indices[5] < wynnitem_list_size(&sortedItems[4]); ++indices[5]) // [4] == rings
//     for (indices[6] = 0; indices[6] < wynnitem_list_size(&sortedItems[5]); ++indices[6])
//     for (indices[7] = 0; indices[7] < wynnitem_list_size(&sortedItems[6]); ++indices[7])
//     for (indices[8] = 0; indices[8] < wynnitem_list_size(&sortedItems[7]); ++indices[8])
//     {
//         printf("t");
//         size_t iterIndices[] = {0, 1, 2, 3, 4, 4, 5, 6, 7}; // Double 4
//         for (size_t i = 0; i < lengthof(iterIndices); ++i)
//         {
//             size_t itemIndex = iterIndices[i];
//             WynnItemIdArray* pIdArray = &wynnitem_list_get(&sortedItems[itemIndex], indices[i])->idArray;
//             float d = 0.f;
//             for (size_t j = 0; j < WYNNITEM_ID_ARRAY_SIZE; ++j)
//             {
//                 d += distanceSqrd(lerp(mins[j], maxs[j], targetValues[j]), (*pIdArray)[j]);
//             }
//             d = sqrtf(d);

//             if (closestScore < d) continue;

//             closestScore = d;
//             memcpy(bestIndices, indices, sizeof(bestIndices));
//         }
//     }

//     return (WynnBuild){0};
// }