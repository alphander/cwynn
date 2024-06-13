#include "itemscores.h"
#include <string.h>

static int wynnitem_score_cmp(const struct scored_item* pItemA, const struct scored_item* pItemB)
{
    return pItemA->score < pItemB->score ? -1 : 1;
}

void scored_items_print(WynnItem* pSearchItem, WynnItemList* pItemList)
{
    ItemScoreHeap itemScores = itemscore_heap_create(wynnitem_score_cmp);
    WynnItem* pItem = NULL;
    while (wynnitem_list_iter_next(pItemList, &pItem))
    {
        float score = wynnitem_similarity(pSearchItem, pItem);
        itemscore_heap_push(&itemScores, (struct scored_item){score, pItem});
    }

    for (int i = 0; itemscore_heap_size(&itemScores) > 0 && i < 20;)
    {
        struct scored_item scoredItem = itemscore_heap_pop(&itemScores);
        if (scoredItem.pItem->type != pSearchItem->type) continue;
        if (!strcmp(scoredItem.pItem->pName->str, pSearchItem->pName->str)) continue;

        printf("  %s %f\n", scoredItem.pItem->pName->str, scoredItem.score);
        i++;
    }
    printf("\n");
    itemscore_heap_destroy(&itemScores);
}