#ifndef ITEMSCORES_H
#define ITEMSCORES_H

#include "wynnitems.h"

struct scored_item
{
    float score;
    WynnItem* pItem;
};

LIST_GENERIC_EX(struct scored_item, ItemScoreList, itemscore_list);

void scored_items_print(WynnItem* pSearchItem, WynnItemList* pItemList);

#endif // ITEMSCORES_H