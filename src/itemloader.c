#include "itemloader.h"
#include <LTK/dataio.h>
#include <LTK/jsonparser.h>
#include <LTK/error_handling.h>
#include <LTK/ansi_codes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

static WynnItemPool gItemPool = {0};
static WynnItemNamePool gNamePool = {0};
static WynnItemList gItemList = {0};
static bool isInit = false;

static WynnItemList wynnitems_load_json(
    JsonValue* pRoot, 
    WynnItemPool* pItemPool, 
    WynnItemNamePool* pNamePool);
static WynnItemList wynnitem_load_bin(
    uint8_t* pData,
    WynnItemPool* pItemPool, 
    WynnItemNamePool* pNamePool);
static uint8_t* wynnitem_dump_bin(WynnItemList* pItemList, size_t* pSizeOut);

static inline uint64_t get_timing()
{
    struct timespec spec;
    timespec_get(&spec, TIME_UTC);

    return spec.tv_nsec + spec.tv_sec * 1000000000ULL;
}

static inline double timing_to_float(uint64_t start, uint64_t end)
{
    return ((double)(end - start)) / 1000000000.0;    
}

WynnItemList* wynnitems_load(char* dbBinPath, char* dbUrl)
{
    ERR_RET(isInit, ERR_FAILURE, NULL);

    gItemPool = wynnitem_pool_create();
    gNamePool = wynnitem_name_pool_create();

    uint64_t timeStart, timeEnd;
    if (dataio_isfile(dbBinPath))
    {
        printf(YELLOW"Reading item database from (%s)...", dbBinPath);
        timeStart = get_timing();
        uint8_t* pData = dataio_read(dbBinPath, NULL);
        gItemList = wynnitem_load_bin(pData, &gItemPool, &gNamePool);
        free(pData);
        timeEnd = get_timing();
        printf(GREEN"Completed: %.3lfs\n"RESET, timing_to_float(timeStart, timeEnd));
    }
    else
    {
        printf(YELLOW"Downloading item database from (%s)...", dbUrl);
        timeStart = get_timing();
        size_t size = 0;
        char* jsonString = (char*)dataio_get(dbUrl, &size);
        timeEnd = get_timing();
        printf(GREEN"Completed: %.3lfs\n"RESET, timing_to_float(timeStart, timeEnd));

        printf(YELLOW"Parsing json...");
        timeStart = get_timing();
        JsonPool jsonPool = json_pool_create();
        JsonValue* pRoot = json_parse(&jsonPool, jsonString);
        free(jsonString);
        timeEnd = get_timing();
        printf(GREEN"Completed: %.3lfs\n"RESET, timing_to_float(timeStart, timeEnd));

        printf(YELLOW"Loading items from json value tree...");
        timeStart = get_timing();
        gItemList = wynnitems_load_json(pRoot, &gItemPool, &gNamePool);
        json_pool_destroy(&jsonPool);
        timeEnd = get_timing();
        printf(GREEN"Completed: %.3lfs\n"RESET, timing_to_float(timeStart, timeEnd));

        printf(YELLOW"Writing item database to (%s)...", dbBinPath);
        timeStart = get_timing();
        size_t writeSize = 0;
        uint8_t* pWriteData = wynnitem_dump_bin(&gItemList, &writeSize);
        dataio_write(dbBinPath, pWriteData, writeSize);
        free(pWriteData);
        timeEnd = get_timing();
        printf(GREEN"Completed: %.3lfs\n"RESET, timing_to_float(timeStart, timeEnd));
    }

    isInit = true;

    return &gItemList;
}

void wynnitems_unload()
{
    ERR_RET(!isInit, ERR_FAILURE,);

    wynnitem_list_destroy(&gItemList);
    wynnitem_name_pool_destroy(&gNamePool);
    wynnitem_pool_destroy(&gItemPool);
}

// ##########################################################################################
// Static functions
//
//
//
//
// ##########################################################################################

inline static int32_t min_max_entry(JsonValue* pSection, const char* keyString)
{
    JsonKey key = {0};
    strcpy_s(key.str, sizeof(key.str), keyString);

    JsonValue* pHealth = json_object_get(pSection, key);
    if (pHealth == NULL) return 0;

    if (json_is_type(pHealth, JSON_VALUE_TYPE_NUMBER))
        return (int32_t)json_to_number(pHealth);

    return (int32_t)json_to_number(json_object_get(pHealth, (JsonKey){"max"}));
}

static WynnItemList wynnitems_load_json(
    JsonValue* pRoot, 
    WynnItemPool* pItemPool, 
    WynnItemNamePool* pNamePool)
{   
    WynnItemList itemList = wynnitem_list_create();

    JsonValue* pJsonItem = NULL;
    JsonKey itemName = {0};
    while ((pJsonItem = json_object_next(pRoot, &itemName)))
    {   
        WynnItem item = {0};
        JsonValue* pTypeStr = json_object_get(pJsonItem, (JsonKey){"type"});
        const char* type = json_to_string(pTypeStr);
        if (!strcmp(type, "weapon"))
        {
            item.type = WYNNITEM_TYPE_WEAPON;
        }
        else if (!strcmp(type, "armour"))
        {
            JsonValue* pArmourTypeValue = json_object_get(pJsonItem, (JsonKey){"armourType"});
            const char* armourType = json_to_string(pArmourTypeValue);
            if (!strcmp(armourType, "helmet")) item.type = WYNNITEM_TYPE_HELMET;
            else if (!strcmp(armourType, "chestplate")) item.type = WYNNITEM_TYPE_CHESTPLATE;
            else if (!strcmp(armourType, "leggings")) item.type = WYNNITEM_TYPE_LEGGINGS;
            else if (!strcmp(armourType, "boots")) item.type = WYNNITEM_TYPE_BOOTS;
            else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
        }
        else continue;
            // else if (!strcmp(type, "ring")) item.type = WYNNITEM_TYPE_RING;
            // else if (!strcmp(type, "bracelet")) item.type = WYNNITEM_TYPE_BRACELET;
            // else if (!strcmp(type, "necklace")) item.type = WYNNITEM_TYPE_NECKLACE;
        

        JsonValue* pRarityStr = json_object_get(pJsonItem, (JsonKey){"rarity"});
        const char* rarity = json_to_string(pRarityStr);
        if (!strcmp(rarity, "common")) item.tier = WYNNITEM_TIER_COMMON;
        else if (!strcmp(rarity, "unique")) item.tier = WYNNITEM_TIER_UNIQUE;
        else if (!strcmp(rarity, "rare")) item.tier = WYNNITEM_TIER_RARE;
        else if (!strcmp(rarity, "legendary")) item.tier = WYNNITEM_TIER_LEGENDARY;
        else if (!strcmp(rarity, "fabled")) item.tier = WYNNITEM_TIER_FABLED;
        else if (!strcmp(rarity, "mythic")) item.tier = WYNNITEM_TIER_MYTHIC;
        else if (!strcmp(rarity, "set")) item.tier = WYNNITEM_TIER_SET;
        else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});

        JsonValue* pAttackSpeedStr = json_object_get(pJsonItem, (JsonKey){"attackSpeed"});
        if (pAttackSpeedStr != NULL)
        {
            const char* speed = json_to_string(pAttackSpeedStr);
            if (!strcmp(speed, "super_slow")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_SUPER_SLOW;
            else if (!strcmp(speed, "very_slow")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_VERY_SLOW;
            else if (!strcmp(speed, "slow")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_SLOW;
            else if (!strcmp(speed, "normal")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_NORMAL;
            else if (!strcmp(speed, "fast")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_FAST;
            else if (!strcmp(speed, "very_fast")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_VERY_FAST;
            else if (!strcmp(speed, "super_fast")) item.attackSpeed = WYNNITEM_ATTACK_SPEED_SUPER_FAST;
            else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
        }

        JsonValue* pReqsSection = json_object_get(pJsonItem, (JsonKey){"requirements"});
        if (pReqsSection != NULL)
        {
            JsonValue* pClassReq = json_object_get(pJsonItem, (JsonKey){"classRequirement"});
            if (pClassReq != NULL)
            {
                const char* class = json_to_string(pClassReq);
                if (!strcmp(class, "mage")) item.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "warrior")) item.class = WYNNITEM_CLASS_WARRIOR;
                else if (!strcmp(class, "archer")) item.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "assassin")) item.class = WYNNITEM_CLASS_ASSASSIN;
                else if (!strcmp(class, "shaman")) item.class = WYNNITEM_CLASS_SHAMAN;
                else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
            }

            item.reqs.level = min_max_entry(pReqsSection, "level");
            item.reqs.strength = min_max_entry(pReqsSection, "strength");
            item.reqs.dexterity = min_max_entry(pReqsSection, "dexterity");
            item.reqs.defence = min_max_entry(pReqsSection, "defence");
            item.reqs.agility = min_max_entry(pReqsSection, "agility");
            item.reqs.intelligence = min_max_entry(pReqsSection, "intelligence");
        }

        JsonValue* pBaseSection = json_object_get(pJsonItem, (JsonKey){"base"});
        if (pBaseSection != NULL)
        {
            item.base.averageDPS = min_max_entry(pBaseSection, "averageDPS");
            item.base.damage = min_max_entry(pBaseSection, "damage");
            item.base.earthDamage = min_max_entry(pBaseSection, "earthDamage");
            item.base.thunderDamage = min_max_entry(pBaseSection, "thunderDamage");
            item.base.fireDamage = min_max_entry(pBaseSection, "fireDamage");
            item.base.airDamage = min_max_entry(pBaseSection, "airDamage");
            item.base.waterDamage = min_max_entry(pBaseSection, "waterDamage");

            item.base.health = min_max_entry(pBaseSection, "health");
            item.base.earthDefence = min_max_entry(pBaseSection, "earthDefence");
            item.base.thunderDefence = min_max_entry(pBaseSection, "thunderDefence");
            item.base.fireDefence = min_max_entry(pBaseSection, "fireDefence");
            item.base.airDefence = min_max_entry(pBaseSection, "airDefence");
            item.base.waterDefence = min_max_entry(pBaseSection, "waterDefence");
        }

        JsonValue* pIdsSection = json_object_get(pJsonItem, (JsonKey){"identifications"});
        if (pIdsSection != NULL)
        {
            item.ids.rawAttackSpeed = min_max_entry(pIdsSection, "rawAttackSpeed");

            item.ids.rawStrength = min_max_entry(pIdsSection, "rawStrength");
            item.ids.rawDexterity = min_max_entry(pIdsSection, "rawDexterity");
            item.ids.rawDefence = min_max_entry(pIdsSection, "rawDefence");
            item.ids.rawAgility = min_max_entry(pIdsSection, "rawAgility");
            item.ids.rawIntelligence = min_max_entry(pIdsSection, "rawIntelligence");

            item.ids.elementalDamage = min_max_entry(pIdsSection, "elementalDamage");
            item.ids.earthDamage = min_max_entry(pIdsSection, "earthDamage");
            item.ids.thunderDamage = min_max_entry(pIdsSection, "thunderDamage");
            item.ids.fireDamage = min_max_entry(pIdsSection, "fireDamage");
            item.ids.airDamage = min_max_entry(pIdsSection, "airDamage");
            item.ids.waterDamage = min_max_entry(pIdsSection, "waterDamage");

            item.ids.elementalDefence = min_max_entry(pIdsSection, "elementalDefence");
            item.ids.earthDefence = min_max_entry(pIdsSection, "earthDefence");
            item.ids.airDefence = min_max_entry(pIdsSection, "airDefence");
            item.ids.fireDefence = min_max_entry(pIdsSection, "fireDefence");
            item.ids.thunderDefence = min_max_entry(pIdsSection, "thunderDefence");
            item.ids.waterDefence = min_max_entry(pIdsSection, "waterDefence");

            item.ids.rawElementalDamage = min_max_entry(pIdsSection, "rawElementalDamage");
            item.ids.rawEarthDamage = min_max_entry(pIdsSection, "rawEarthDamage");
            item.ids.rawThunderDamage = min_max_entry(pIdsSection, "rawThunderDamage");
            item.ids.rawFireDamage = min_max_entry(pIdsSection, "rawFireDamage");
            item.ids.rawAirDamage = min_max_entry(pIdsSection, "rawAirDamage");
            item.ids.rawWaterDamage = min_max_entry(pIdsSection, "rawWaterDamage");

            item.ids.mainAttackDamage = min_max_entry(pIdsSection, "mainAttackDamage");
            item.ids.earthMainAttackDamage = min_max_entry(pIdsSection, "earthMainAttackDamage");
            item.ids.thunderMainAttackDamage = min_max_entry(pIdsSection, "thunderMainAttackDamage");
            item.ids.fireMainAttackDamage = min_max_entry(pIdsSection, "fireMainAttackDamage");
            item.ids.airMainAttackDamage = min_max_entry(pIdsSection, "airMainAttackDamage");
            item.ids.waterMainAttackDamage = min_max_entry(pIdsSection, "waterMainAttackDamage");

            item.ids.rawMainAttackDamage = min_max_entry(pIdsSection, "rawMainAttackDamage");
            item.ids.rawElementalMainAttackDamage = min_max_entry(pIdsSection, "rawElementalMainAttackDamage");
            item.ids.rawEarthMainAttackDamage = min_max_entry(pIdsSection, "rawEarthMainAttackDamage");
            item.ids.rawThunderMainAttackDamage = min_max_entry(pIdsSection, "rawThunderMainAttackDamage");
            item.ids.rawFireMainAttackDamage = min_max_entry(pIdsSection, "rawFireMainAttackDamage");
            item.ids.rawAirMainAttackDamage = min_max_entry(pIdsSection, "rawAirMainAttackDamage");
            item.ids.rawWaterMainAttackDamage = min_max_entry(pIdsSection, "rawWaterMainAttackDamage");

            item.ids.spellDamage = min_max_entry(pIdsSection, "spellDamage");
            item.ids.elementalSpellDamage = min_max_entry(pIdsSection, "elementalSpellDamage");
            item.ids.fireSpellDamage = min_max_entry(pIdsSection, "fireSpellDamage");
            item.ids.waterSpellDamage = min_max_entry(pIdsSection, "waterSpellDamage");
            item.ids.earthSpellDamage = min_max_entry(pIdsSection, "earthSpellDamage");
            item.ids.thunderSpellDamage = min_max_entry(pIdsSection, "thunderSpellDamage");
            item.ids.airSpellDamage = min_max_entry(pIdsSection, "airSpellDamage");

            item.ids.rawSpellDamage = min_max_entry(pIdsSection, "rawSpellDamage");
            item.ids.rawNeutralSpellDamage = min_max_entry(pIdsSection, "rawNeutralSpellDamage");
            item.ids.rawElementalSpellDamage = min_max_entry(pIdsSection, "rawElementalSpellDamage");
            item.ids.rawEarthSpellDamage = min_max_entry(pIdsSection, "rawEarthSpellDamage");
            item.ids.rawThunderSpellDamage = min_max_entry(pIdsSection, "rawThunderSpellDamage");
            item.ids.rawFireSpellDamage = min_max_entry(pIdsSection, "rawFireSpellDamage");
            item.ids.rawAirSpellDamage = min_max_entry(pIdsSection, "rawAirSpellDamage");
            item.ids.rawWaterSpellDamage = min_max_entry(pIdsSection, "rawWaterSpellDamage");

            item.ids.healing = min_max_entry(pIdsSection, "healing");
            item.ids.rawHealth = min_max_entry(pIdsSection, "rawHealth");
            item.ids.healingEfficiency = min_max_entry(pIdsSection, "healingEfficiency");
            item.ids.healthRegen = min_max_entry(pIdsSection, "healthRegen");
            item.ids.healthRegenRaw = min_max_entry(pIdsSection, "healthRegenRaw");
            item.ids.lifeSteal = min_max_entry(pIdsSection, "lifeSteal");

            item.ids.manaRegen = min_max_entry(pIdsSection, "manaRegen");
            item.ids.manaSteal = min_max_entry(pIdsSection, "manaSteal");

            item.ids.sprint = min_max_entry(pIdsSection, "sprint");
            item.ids.sprintRegen = min_max_entry(pIdsSection, "sprintRegen");
            item.ids.walkSpeed = min_max_entry(pIdsSection, "walkSpeed");
            item.ids.jumpHeight = min_max_entry(pIdsSection, "jumpHeight");

            item.ids.thorns = min_max_entry(pIdsSection, "thorns");
            item.ids.reflection = min_max_entry(pIdsSection, "reflection");
            item.ids.knockback = min_max_entry(pIdsSection, "knockback");
            item.ids.exploding = min_max_entry(pIdsSection, "exploding");
            item.ids.poison = min_max_entry(pIdsSection, "poison");

            item.ids.stealing = min_max_entry(pIdsSection, "stealing");
            item.ids.xpBonus = min_max_entry(pIdsSection, "xpBonus");
            item.ids.lootBonus = min_max_entry(pIdsSection, "lootBonus");
            item.ids.soulPointRegen = min_max_entry(pIdsSection, "soulPointRegen");

            item.ids.SpellCost1st = min_max_entry(pIdsSection, "1stSpellCost");
            item.ids.SpellCost2nd = min_max_entry(pIdsSection, "2ndSpellCost");
            item.ids.SpellCost3rd = min_max_entry(pIdsSection, "3rdSpellCost");
            item.ids.SpellCost4th = min_max_entry(pIdsSection, "4thSpellCost");

            item.ids.rawSpellCost1st = min_max_entry(pIdsSection, "raw1stSpellCost");
            item.ids.rawSpellCost2nd = min_max_entry(pIdsSection, "raw2ndSpellCost");
            item.ids.rawSpellCost3rd = min_max_entry(pIdsSection, "raw3rdSpellCost");
            item.ids.rawSpellCost4th = min_max_entry(pIdsSection, "raw4thSpellCost");

            item.ids.slowEnemy = min_max_entry(pIdsSection, "slowEnemy");
            item.ids.weakenEnemy = min_max_entry(pIdsSection, "weakenEnemy");
        }

        item.pName = wynnitem_name_pool_alloc(pNamePool);
        memset(item.pName->str, 0, sizeof(item.pName->str));
        strncpy_s(item.pName->str, sizeof(item.pName->str), itemName.str, sizeof(itemName.str));

        WynnItem* pItem = wynnitem_pool_alloc(pItemPool);
        *pItem = item;

        wynnitem_list_append(&itemList, pItem);
    }
    return itemList;
}

#pragma pack(push, 1)
struct wynnitem_bin_header
{
    uint32_t count;
};

struct wynnitem_bin_item 
{
    char name[64];
    WynnItemType type;
    WynnItemTier tier;
    WynnItemClass class;
    uint8_t powderSlots;
    int32_t attackSpeed;
    WynnItemIdArray idArray;
};
#pragma pack(pop)

static uint8_t* wynnitem_dump_bin(WynnItemList* pItemList, size_t* pSizeOut)
{
    size_t size = sizeof(struct wynnitem_bin_header);
    size += wynnitem_list_size(pItemList) * sizeof(struct wynnitem_bin_item);

    uint8_t* pBuffer = malloc(size);
    struct wynnitem_bin_header* pHeader = (struct wynnitem_bin_header*)pBuffer;
    struct wynnitem_bin_item* pItemArray = (struct wynnitem_bin_item*)(pBuffer + sizeof(struct wynnitem_bin_header));

    pHeader->count = (uint32_t)wynnitem_list_size(pItemList);

    for (size_t i = 0; i < wynnitem_list_size(pItemList); i++)
    {
        WynnItem* pItem = wynnitem_list_get(pItemList, i);
        struct wynnitem_bin_item* pBinItem = pItemArray + i;
        strncpy_s(
            pBinItem->name, sizeof(pBinItem->name), 
            pItem->pName->str, sizeof(pItem->pName->str)
        );
        pBinItem->type = pItem->type;
        pBinItem->tier = pItem->tier;
        pBinItem->powderSlots = pItem->powderSlots;
        pBinItem->attackSpeed = pItem->attackSpeed;
        pBinItem->class = pItem->class;
        memcpy_s(
            pBinItem->idArray, sizeof(pBinItem->idArray), 
            pItem->idArray, sizeof(pItem->idArray)
        );
    }

    if (pSizeOut != NULL)
        *pSizeOut = size;

    return pBuffer;
}

static WynnItemList wynnitem_load_bin(
    uint8_t* pData,
    WynnItemPool* pItemPool, 
    WynnItemNamePool* pNamePool)
{
    WynnItemList itemList = wynnitem_list_create();

    struct wynnitem_bin_header header = *(struct wynnitem_bin_header*)pData;
    pData += sizeof(struct wynnitem_bin_header);

    for (size_t i = 0; i < header.count; i++)
    {
        struct wynnitem_bin_item* pBinItem = ((struct wynnitem_bin_item*)pData) + i;
        WynnItem* pItem = wynnitem_pool_alloc(pItemPool);
        pItem->pName = wynnitem_name_pool_alloc(pNamePool);
        strncpy_s(
            pItem->pName->str, sizeof(pItem->pName->str), 
            pBinItem->name, sizeof(pBinItem->name)
        );
        pItem->type = pBinItem->type;
        pItem->tier = pBinItem->tier;
        pItem->powderSlots = pBinItem->powderSlots;
        pItem->attackSpeed = pBinItem->attackSpeed;
        memcpy_s(
            pItem->idArray, sizeof(pItem->idArray), 
            pBinItem->idArray, sizeof(pItem->idArray)
        );

        wynnitem_list_append(&itemList, pItem);
    }

    return itemList;
}