#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <LTK/containers.h>
#include <LTK/threading.h>
#include "interface.h"
#include "wynnitems.h"
#include "itemloader.h"
#include "itemsearch.h"

#define DB_URL "https://api.wynncraft.com/v3/item/database?fullResult"
#define DB_BIN_PATH "data/wynnitems.bin"

int main(int argc, char* argv[])
{

    WynnItemList* pItemList = wynnitems_load(DB_BIN_PATH, DB_URL);
    wynnitems_init(pItemList);

    itemsearch_start(pItemList);

    // iteminterface_run(NULL);

    // WynnBuild bestBuild = wynnitems_calculate_build(100000);

    // for (size_t i = 0; i < 9; ++i)
    // {
    //     printf("%s\n", bestBuild.pItems[i]->pName->str);
    // }
    // printf("\n");

    // thread_wait(&thrd);
    wynnitems_cleanup(); 
    wynnitems_unload();
    return 0;
}
