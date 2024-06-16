#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <LTK/containers.h>
#include <LTK/threading.h>
#include "iteminterface.h"
#include "wynnitems.h"
#include "itemloader.h"
#include "itemsearch.h"

#define DB_URL "https://api.wynncraft.com/v3/item/database?fullResult"
#define DB_BIN_PATH "data/wynnitems.bin"

int main(int argc, char* argv[])
{
    iteminterface_run(NULL);

    WynnItemList* pItemList = wynnitems_load(DB_BIN_PATH, DB_URL);

    // itemsearch_start(pItemList);
    // thread_wait(&thrd);

    wynnitems_unload();
    return 0;
}
