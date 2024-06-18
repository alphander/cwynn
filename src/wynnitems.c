#include "wynnitems.h"
#include <math.h>

static inline float stat_distance(int32_t a, int32_t b)
{
    return ((float)b - a) * ((float)b - a);
}

float wynnitem_similarity(WynnItem* pItem, WynnItem* pTestItem)
{
    float v = 0;

    size_t count = sizeof(pItem->idArray) / sizeof(*pItem->idArray);
    for (size_t i = 0; i < count; i++)
    {
        v += stat_distance(pItem->idArray[i], pTestItem->idArray[i]);
    }

    return sqrtf(v);
}