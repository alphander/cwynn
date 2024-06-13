#include "itemlevenshtein.h"
#include <string.h>
#include "wynnitems.h"

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