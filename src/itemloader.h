#ifndef ITEMLOADER_H
#define ITEMLOADER_H

#include "wynnitems.h"

WynnItemList* wynnitems_load(char* dbBinPath, char* dbUrl);
void wynnitems_unload();

#endif // ITEMLOADER_H