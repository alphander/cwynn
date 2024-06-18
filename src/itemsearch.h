#ifndef ITEMSEARCH_H
#define ITEMSEARCH_H

#include "wynnitems.h"

struct levenshtein_item
{
    uint32_t distance;
    WynnItem* pItem;
};

HEAP_GENERIC_EX(struct levenshtein_item, LevenshteinHeap, levenshtein_heap);

uint32_t levenshtein(char* str, char* testStr);
LevenshteinHeap levenshtein_sorted(char* itemName, WynnItemList* pItemList);

struct scored_item
{
    float score;
    WynnItem* pItem;
};

HEAP_GENERIC_EX(struct scored_item, ItemScoreHeap, itemscore_heap);

void scored_items_print(WynnItem* pSearchItem, WynnItemList* pItemList);
void itemsearch_start(WynnItemList* pItemList);

#endif // ITEMSEARCH_H