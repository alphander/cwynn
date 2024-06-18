#include "itemsearch.h"
#include <string.h>
#include <ctype.h>
#include "wynnitems.h"
#include "itemloader.h"

// ################################################################################
// Levenshtein distance
// "How do Spell Checkers work? Levenshtein Edit Distance" 
// Creel: (https://www.youtube.com/watch?v=Cu7Tl7FGigQ)
//
// ################################################################################

;

uint32_t levenshtein(char* str, char* testStr)
{
    size_t strLen = strnlen(str, sizeof(WynnItemName)) + 1;
    size_t testStrLen = strnlen(testStr, sizeof(WynnItemName)) + 1;

    char* strA = strLen > testStrLen ? str : testStr;
    size_t strALen = strLen > testStrLen ? strLen : testStrLen;
    char* strB = strLen > testStrLen ? testStr : str;
    size_t strBLen = strLen > testStrLen ? testStrLen : strLen;

    uint32_t array[strBLen];
    for (size_t i = 0; i < strBLen; ++i) array[i] = i;

    uint32_t sub;
    for (size_t i = 1; i < strALen; ++i)
    {
        sub = array[0];
        array[0] = i;
        for (size_t j = 1; j < strBLen; ++j) 
        {
            uint32_t ins = array[j];
            uint32_t del = array[j - 1];
            
            uint32_t acc = sub;
            if (strA[i - 1] != strB[j - 1])
            {
                acc = ins < del ? ins : del;
                acc = sub < acc ? sub : acc;
                ++acc;
            }
            sub = array[j];
            array[j] = acc;
        }
    }
    return array[strBLen - 1];
}

static int levenshtein_cmp(
    const struct levenshtein_item* pItemA, 
    const struct levenshtein_item* pItemB)
{
    return pItemA->distance < pItemB->distance ? -1 : 1;
}

LevenshteinHeap levenshtein_sorted(char* itemName, WynnItemList* pItemList)
{
    LevenshteinHeap heap = levenshtein_heap_create(levenshtein_cmp);

    WynnItem* pItem = NULL;
    while (wynnitem_list_iter_next(pItemList, &pItem))
    {
        float score = levenshtein(itemName, pItem->pName->str);
        levenshtein_heap_push(&heap, (struct levenshtein_item){score, pItem});
    }

    return heap;
}

// ################################################################################
// Item recommender printing
//
//
// ################################################################################



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

static WynnItem* select_search_item(WynnItemList* pItemList)
{
    printf("Search item: ");
    WynnItemName searchName = {0};
    fgets(searchName.str, sizeof(searchName.str), stdin);
    *strchr(searchName.str, '\n') = '\0';

    LevenshteinHeap levenshteinHeap = levenshtein_sorted(searchName.str, pItemList);
    struct levenshtein_item levenshteinItem = levenshtein_heap_peek(&levenshteinHeap);
    WynnItem* pBestMatchItem = levenshteinItem.pItem;
    uint32_t lowestDist = levenshteinItem.distance;

    if (lowestDist == 0)
    {
        levenshtein_heap_destroy(&levenshteinHeap);
        return pBestMatchItem;
    }

    for (;;)
    {
        WynnItem* pItems[8];
        for (size_t i = 0; i < 8; i++)
        {
            pItems[i] = levenshtein_heap_pop(&levenshteinHeap).pItem;
            printf("%zu %s\n", i + 1, pItems[i]->pName->str);
        }

        printf ("Did you mean? ([1-9]/n): ");
        char answer[4]; 
        fgets(answer, sizeof(answer), stdin);
        WynnItem* pReturnItem = NULL;

        if (strnlen(answer, sizeof(answer)) > 2)
        {
            printf("Input is more than a single char!\n");
            continue;
        }
        else if (strnlen(answer, sizeof(answer)) < 2)
            pReturnItem = pItems[0];
        else if (answer[0] >= '1' && answer[0] <= '8')
            pReturnItem = pItems[answer[0] - '1'];
        else if (tolower(answer[0]) == 'n')
            pReturnItem = NULL;
        else
        {
            printf("Input is not 1-9 or 'n'!\n");
            continue;
        }
        
        levenshtein_heap_destroy(&levenshteinHeap);

        return pReturnItem;
    }
}

void itemsearch_start(WynnItemList* pItemList)
{
    for (;;)
    {
        WynnItem* pSearchItem = select_search_item(pItemList);
        if (pSearchItem == NULL) continue;

        printf("Selected: '%s'\n", pSearchItem->pName->str);
        scored_items_print(pSearchItem, pItemList);
    }
}