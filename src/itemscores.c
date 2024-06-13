#include "itemscores.h"
#include <string.h>

static int wynnitem_score_cmp(const struct scored_item* pItemA, const struct scored_item* pItemB)
{
    return pItemA->score < pItemB->score ? -1 : 1;
}

void scored_items_print(WynnItem* pSearchItem, WynnItemList* pItemList)
{
    ItemScoreList itemScores = itemscore_list_create();
    WynnItem* pItem = NULL;
    while (wynnitem_list_iter_next(pItemList, &pItem))
    {
        float score = wynnitem_similarity(pSearchItem, pItem);
        itemscore_list_append(&itemScores, (struct scored_item){score, pItem});
    }

    itemscore_list_sort(&itemScores, wynnitem_score_cmp);

    struct scored_item scoredItem = {0};
    for (int i = 0; itemscore_list_iter_next(&itemScores, &scoredItem) && i < 20;)
    {
        if (scoredItem.pItem->type != pSearchItem->type) continue;
        if (!strcmp(scoredItem.pItem->pName->str, pSearchItem->pName->str)) continue;

        printf("  %s %f\n", scoredItem.pItem->pName->str, scoredItem.score);
        i++;
    }
    printf("\n");
    itemscore_list_destroy(&itemScores);
}