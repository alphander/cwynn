#ifndef ITEMLEVENSHTEIN_H
#define ITEMLEVENSHTEIN_H

#include <stdint.h>
#include <LTK/containers.h>
#include "wynnitems.h"

struct levenshtein_item
{
    uint32_t distance;
    WynnItem* pItem;
};

LIST_GENERIC_EX(struct levenshtein_item, LevenshteinList, levenshtein_list);

uint32_t levenshtein(char* str, char* testStr);
LevenshteinList levenshtein_sorted(char* itemName, WynnItemList* pItemList);

#endif // ITEMLEVENSHTEIN_H