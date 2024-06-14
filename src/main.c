#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <LTK/containers.h>
#include "wynnitems.h"
#include "itemloader.h"
#include "itemscores.h"
#include "itemlevenshtein.h"

#define DB_URL "https://api.wynncraft.com/v3/item/database?fullResult"
#define DB_BIN_PATH "data/wynnitems.bin"

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

int main(int argc, char* argv[])
{
    WynnItemList* pItemList = wynnitems_load(DB_BIN_PATH, DB_URL);

    for (;;)
    {
        WynnItem* pSearchItem = select_search_item(pItemList);
        if (pSearchItem == NULL) continue;

        printf("Selected: '%s'\n", pSearchItem->pName->str);
        scored_items_print(pSearchItem, pItemList);
    }

    wynnitems_unload();
    return 0;
}
