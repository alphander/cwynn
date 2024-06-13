#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <LTK/containers.h>
#include "wynnitems.h"
#include "itemloader.h"
#include "itemscores.h"
#include "itemlevenshtein.h"

#define DB_URL "https://api.wynncraft.com/v3/item/database?fullResult"
#define DB_PATH "data/wynndata.json"

static WynnItem* select_search_item(WynnItemList* pItemList)
{
    printf("Search item: ");
    WynnItemName searchName = {0};
    fgets(searchName.str, sizeof(searchName.str), stdin);
    *strchr(searchName.str, '\n') = '\0';

    LevenshteinList levenshteinList = levenshtein_sorted(searchName.str, pItemList);
    struct levenshtein_item levenshteinItem = levenshtein_list_get(&levenshteinList, 0);
    WynnItem* pBestMatchItem = levenshteinItem.pItem;
    uint32_t lowestDist = levenshteinItem.distance;

    if (lowestDist == 0)
    {
        levenshtein_list_destroy(&levenshteinList);
        return pBestMatchItem;
    }

    for (;;)
    {
        for (size_t i = 0; i < 8; i++)
        {
            char* itemName = levenshtein_list_get(&levenshteinList, i).pItem->pName->str;
            printf("%zu %s\n", i + 1, itemName);
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
            pReturnItem = levenshtein_list_get(&levenshteinList, 0).pItem;
        else if (answer[0] >= '1' && answer[0] <= '8')
            pReturnItem = levenshtein_list_get(&levenshteinList, answer[0] - '1').pItem;
        else if (tolower(answer[0]) == 'n')
            pReturnItem = NULL;
        else
        {
            printf("Input is not 1-9 or 'n'!\n");
            continue;
        }
        
        levenshtein_list_destroy(&levenshteinList);

        return pReturnItem;
    }
}

int main(int argc, char* argv[])
{
    WynnItemPool itemPool = wynnitem_pool_create();
    NamePool namePool = name_pool_create();
    WynnItemList itemList = wynnitems_load(&itemPool, &namePool, DB_PATH, DB_URL);

    for (;;)
    {
        WynnItem* pSearchItem = select_search_item(&itemList);
        if (pSearchItem == NULL) continue;

        printf("Selected: '%s'\n", pSearchItem->pName->str);

        scored_items_print(pSearchItem, &itemList);
    }

    wynnitem_list_destroy(&itemList);
    wynnitem_pool_destroy(&itemPool);
    name_pool_destroy(&namePool);

    return 0;
}
