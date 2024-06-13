#ifndef ITEMLOADER_H
#define ITEMLOADER_H

#include "wynnitems.h"

WynnItemList wynnitems_load(WynnItemPool* pItemPool, NamePool* pNamePool, char* dbPath, char* dbUrl);

#endif // ITEMLOADER_H