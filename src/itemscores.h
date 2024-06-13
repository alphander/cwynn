#ifndef ITEMSCORES_H
#define ITEMSCORES_H

#include "wynnitems.h"

struct scored_item
{
    float score;
    WynnItem* pItem;
};

HEAP_GENERIC_EX(struct scored_item, ItemScoreHeap, itemscore_heap);

void scored_items_print(WynnItem* pSearchItem, WynnItemList* pItemList);

#endif // ITEMSCORES_H