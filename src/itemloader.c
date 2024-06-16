#include "itemloader.h"
#include <LTK/dataio.h>
#include <LTK/jsonparser.h>
#include <LTK/error_handling.h>
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
        printf("Reading item database from (%s)...", dbBinPath);
        timeStart = get_timing();
        uint8_t* pData = dataio_read(dbBinPath, NULL);
        gItemList = wynnitem_load_bin(pData, &gItemPool, &gNamePool);
        free(pData);
        timeEnd = get_timing();
        printf("Completed: %.3lfs\n", timing_to_float(timeStart, timeEnd));
    }
    else
    {
        printf("Downloading item database from (%s)...", dbUrl);
        timeStart = get_timing();
        size_t size = 0;
        char* jsonString = (char*)dataio_get(dbUrl, &size);
        timeEnd = get_timing();
        printf("Completed: %.3lfs\n", timing_to_float(timeStart, timeEnd));

        printf("Parsing json...");
        timeStart = get_timing();
        JsonPool jsonPool = json_pool_create();
        JsonValue* pRoot = json_parse(&jsonPool, jsonString);
        free(jsonString);
        timeEnd = get_timing();
        printf("Completed: %.3lfs\n", timing_to_float(timeStart, timeEnd));

        printf("Loading items from json value tree...");
        timeStart = get_timing();
        gItemList = wynnitems_load_json(pRoot, &gItemPool, &gNamePool);
        json_pool_destroy(&jsonPool);
        timeEnd = get_timing();
        printf("Completed: %.3lfs\n", timing_to_float(timeStart, timeEnd));

        printf("Writing item database to (%s)...", dbBinPath);
        timeStart = get_timing();
        size_t writeSize = 0;
        uint8_t* pWriteData = wynnitem_dump_bin(&gItemList, &writeSize);
        dataio_write(dbBinPath, pWriteData, writeSize);
        free(pWriteData);
        timeEnd = get_timing();
        printf("Completed: %.3lfs\n", timing_to_float(timeStart, timeEnd));
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
        JsonValue* pTypeStr = NULL;
        if ((pTypeStr = json_object_get(pJsonItem, (JsonKey){"type"})) ||
            (pTypeStr = json_object_get(pJsonItem, (JsonKey){"accessoryType"})))
        {
            const char* type = json_to_string(pTypeStr);
            if (!strcmp(type, "helmet")) item.type = WYNNITEM_TYPE_HELMET;
            else if (!strcmp(type, "chestplate")) item.type = WYNNITEM_TYPE_CHESTPLATE;
            else if (!strcmp(type, "leggings")) item.type = WYNNITEM_TYPE_LEGGINGS;
            else if (!strcmp(type, "boots")) item.type = WYNNITEM_TYPE_BOOTS;
            else if (!strcmp(type, "ring")) item.type = WYNNITEM_TYPE_RING;
            else if (!strcmp(type, "bracelet")) item.type = WYNNITEM_TYPE_BRACELET;
            else if (!strcmp(type, "necklace")) item.type = WYNNITEM_TYPE_NECKLACE;
            else if (!strcmp(type, "spear")) item.type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "bow")) item.type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "wand")) item.type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "dagger")) item.type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "relik")) item.type = WYNNITEM_TYPE_WEAPON;
            else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
        } 
        else continue;

        JsonValue* pTierStr = json_object_get(pJsonItem, (JsonKey){"tier"});
        const char* tier = json_to_string(pTierStr);
        if (!strcmp(tier, "common")) item.tier = WYNNITEM_TIER_COMMON;
        else if (!strcmp(tier, "unique")) item.tier = WYNNITEM_TIER_UNIQUE;
        else if (!strcmp(tier, "rare")) item.tier = WYNNITEM_TIER_RARE;
        else if (!strcmp(tier, "legendary")) item.tier = WYNNITEM_TIER_LEGENDARY;
        else if (!strcmp(tier, "fabled")) item.tier = WYNNITEM_TIER_FABLED;
        else if (!strcmp(tier, "mythic")) item.tier = WYNNITEM_TIER_MYTHIC;
        else if (!strcmp(tier, "set")) item.tier = WYNNITEM_TIER_SET;
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
                if (!strcmp(class, "mage")) item.reqs.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "warrior")) item.reqs.class = WYNNITEM_CLASS_WARRIOR;
                else if (!strcmp(class, "archer")) item.reqs.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "assassin")) item.reqs.class = WYNNITEM_CLASS_ASSASSIN;
                else if (!strcmp(class, "shaman")) item.reqs.class = WYNNITEM_CLASS_SHAMAN;
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
    uint8_t powderSlots;
    int32_t attackSpeed;
    
    // Requirement
    struct {
        WynnItemClass class;
        int32_t level;
        int32_t strength;
        int32_t dexterity;
        int32_t defence;
        int32_t agility;
        int32_t intelligence;
    } reqs;

    // Base
    struct {
        int32_t averageDPS;
        int32_t damage;
        int32_t earthDamage;
        int32_t thunderDamage;
        int32_t fireDamage;
        int32_t airDamage;
        int32_t waterDamage;

        int32_t health;
        int32_t earthDefence;
        int32_t thunderDefence;
        int32_t fireDefence;
        int32_t airDefence;
        int32_t waterDefence;
    } base;

    // Identifications

    struct {
        int32_t rawAttackSpeed;

        int32_t rawStrength;
        int32_t rawDexterity;
        int32_t rawDefence;
        int32_t rawAgility;
        int32_t rawIntelligence;

        int32_t elementalDamage;
        int32_t earthDamage;
        int32_t thunderDamage;
        int32_t fireDamage;
        int32_t airDamage;
        int32_t waterDamage;

        int32_t elementalDefence;
        int32_t earthDefence;
        int32_t thunderDefence;
        int32_t fireDefence;
        int32_t airDefence;
        int32_t waterDefence;

        int32_t rawElementalDamage;
        int32_t rawEarthDamage;
        int32_t rawThunderDamage;
        int32_t rawFireDamage;
        int32_t rawAirDamage;
        int32_t rawWaterDamage;

        int32_t mainAttackDamage;
        int32_t earthMainAttackDamage;
        int32_t thunderMainAttackDamage;
        int32_t fireMainAttackDamage;
        int32_t airMainAttackDamage;
        int32_t waterMainAttackDamage;

        int32_t rawMainAttackDamage;
        int32_t rawElementalMainAttackDamage;
        int32_t rawEarthMainAttackDamage;
        int32_t rawThunderMainAttackDamage;
        int32_t rawFireMainAttackDamage;
        int32_t rawAirMainAttackDamage;
        int32_t rawWaterMainAttackDamage;

        int32_t spellDamage;
        int32_t elementalSpellDamage;
        int32_t earthSpellDamage;
        int32_t thunderSpellDamage;
        int32_t fireSpellDamage;
        int32_t airSpellDamage;
        int32_t waterSpellDamage;

        int32_t rawSpellDamage;
        int32_t rawNeutralSpellDamage;
        int32_t rawElementalSpellDamage;
        int32_t rawEarthSpellDamage;
        int32_t rawThunderSpellDamage;
        int32_t rawFireSpellDamage;
        int32_t rawAirSpellDamage;
        int32_t rawWaterSpellDamage;

        int32_t healing;
        int32_t rawHealth;
        int32_t healingEfficiency;
        int32_t healthRegen;
        int32_t healthRegenRaw;
        int32_t lifeSteal;

        int32_t manaRegen;
        int32_t manaSteal;

        int32_t sprint;
        int32_t sprintRegen;
        int32_t walkSpeed;
        int32_t jumpHeight;

        int32_t thorns;
        int32_t reflection;
        int32_t knockback;
        int32_t exploding;
        int32_t poison;

        int32_t stealing;
        int32_t xpBonus;
        int32_t lootBonus;
        int32_t soulPointRegen;

        int32_t SpellCost1st;
        int32_t SpellCost2nd;
        int32_t SpellCost3rd;
        int32_t SpellCost4th;

        int32_t rawSpellCost1st;
        int32_t rawSpellCost2nd;
        int32_t rawSpellCost3rd;
        int32_t rawSpellCost4th;

        int32_t slowEnemy;
        int32_t weakenEnemy;
    } ids;
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
        
        memcpy_s(pBinItem->name, sizeof(pBinItem->name), pItem->pName->str, sizeof(pItem->pName->str));
        pBinItem->type = pItem->type;
        pBinItem->tier = pItem->tier;
        pBinItem->powderSlots = pItem->powderSlots;
        pBinItem->attackSpeed = pItem->attackSpeed;

        pBinItem->reqs.class = pItem->reqs.class;
        pBinItem->reqs.level = pItem->reqs.level;
        pBinItem->reqs.strength = pItem->reqs.strength;
        pBinItem->reqs.dexterity = pItem->reqs.dexterity;
        pBinItem->reqs.defence = pItem->reqs.defence;
        pBinItem->reqs.agility = pItem->reqs.agility;
        pBinItem->reqs.intelligence = pItem->reqs.intelligence;

        pBinItem->base.averageDPS = pItem->base.averageDPS;
        pBinItem->base.damage = pItem->base.damage;
        pBinItem->base.earthDamage = pItem->base.earthDamage;
        pBinItem->base.thunderDamage = pItem->base.thunderDamage;
        pBinItem->base.fireDamage = pItem->base.fireDamage;
        pBinItem->base.airDamage = pItem->base.airDamage;
        pBinItem->base.waterDamage = pItem->base.waterDamage;

        pBinItem->base.health = pItem->base.health;
        pBinItem->base.earthDefence = pItem->base.earthDefence;
        pBinItem->base.thunderDefence = pItem->base.thunderDefence;
        pBinItem->base.fireDefence = pItem->base.fireDefence;
        pBinItem->base.airDefence = pItem->base.airDefence;
        pBinItem->base.waterDefence = pItem->base.waterDefence;

        pBinItem->ids.rawAttackSpeed = pItem->ids.rawAttackSpeed;
        pBinItem->ids.rawStrength = pItem->ids.rawStrength;
        pBinItem->ids.rawDexterity = pItem->ids.rawDexterity;
        pBinItem->ids.rawDefence = pItem->ids.rawDefence;
        pBinItem->ids.rawAgility = pItem->ids.rawAgility;
        pBinItem->ids.rawIntelligence = pItem->ids.rawIntelligence;

        pBinItem->ids.elementalDamage = pItem->ids.elementalDamage;
        pBinItem->ids.earthDamage = pItem->ids.earthDamage;
        pBinItem->ids.thunderDamage = pItem->ids.thunderDamage;
        pBinItem->ids.fireDamage = pItem->ids.fireDamage;
        pBinItem->ids.airDamage = pItem->ids.airDamage;
        pBinItem->ids.waterDamage = pItem->ids.waterDamage;

        pBinItem->ids.elementalDefence = pItem->ids.elementalDefence;
        pBinItem->ids.earthDefence = pItem->ids.earthDefence;
        pBinItem->ids.thunderDefence = pItem->ids.thunderDefence;
        pBinItem->ids.fireDefence = pItem->ids.fireDefence;
        pBinItem->ids.airDefence = pItem->ids.airDefence;
        pBinItem->ids.waterDefence = pItem->ids.waterDefence;

        pBinItem->ids.rawElementalDamage = pItem->ids.rawElementalDamage;
        pBinItem->ids.rawEarthDamage = pItem->ids.rawEarthDamage;
        pBinItem->ids.rawThunderDamage = pItem->ids.rawThunderDamage;
        pBinItem->ids.rawFireDamage = pItem->ids.rawFireDamage;
        pBinItem->ids.rawAirDamage = pItem->ids.rawAirDamage;
        pBinItem->ids.rawWaterDamage = pItem->ids.rawWaterDamage;

        pBinItem->ids.mainAttackDamage = pItem->ids.mainAttackDamage;
        pBinItem->ids.earthMainAttackDamage = pItem->ids.earthMainAttackDamage;
        pBinItem->ids.thunderMainAttackDamage = pItem->ids.thunderMainAttackDamage;
        pBinItem->ids.fireMainAttackDamage = pItem->ids.fireMainAttackDamage;
        pBinItem->ids.airMainAttackDamage = pItem->ids.airMainAttackDamage;
        pBinItem->ids.waterMainAttackDamage = pItem->ids.waterMainAttackDamage;

        pBinItem->ids.rawMainAttackDamage = pItem->ids.rawMainAttackDamage;
        pBinItem->ids.rawElementalMainAttackDamage = pItem->ids.rawElementalMainAttackDamage;
        pBinItem->ids.rawEarthMainAttackDamage = pItem->ids.rawEarthMainAttackDamage;
        pBinItem->ids.rawThunderMainAttackDamage = pItem->ids.rawThunderMainAttackDamage;
        pBinItem->ids.rawFireMainAttackDamage = pItem->ids.rawFireMainAttackDamage;
        pBinItem->ids.rawAirMainAttackDamage = pItem->ids.rawAirMainAttackDamage;
        pBinItem->ids.rawWaterMainAttackDamage = pItem->ids.rawWaterMainAttackDamage;

        pBinItem->ids.spellDamage = pItem->ids.spellDamage;
        pBinItem->ids.elementalSpellDamage = pItem->ids.elementalSpellDamage;
        pBinItem->ids.earthSpellDamage = pItem->ids.earthSpellDamage;
        pBinItem->ids.thunderSpellDamage = pItem->ids.thunderSpellDamage;
        pBinItem->ids.fireSpellDamage = pItem->ids.fireSpellDamage;
        pBinItem->ids.airSpellDamage = pItem->ids.airSpellDamage;
        pBinItem->ids.waterSpellDamage = pItem->ids.waterSpellDamage;

        pBinItem->ids.rawSpellDamage = pItem->ids.rawSpellDamage;
        pBinItem->ids.rawNeutralSpellDamage = pItem->ids.rawNeutralSpellDamage;
        pBinItem->ids.rawElementalSpellDamage = pItem->ids.rawElementalSpellDamage;
        pBinItem->ids.rawEarthSpellDamage = pItem->ids.rawEarthSpellDamage;
        pBinItem->ids.rawThunderSpellDamage = pItem->ids.rawThunderSpellDamage;
        pBinItem->ids.rawFireSpellDamage = pItem->ids.rawFireSpellDamage;
        pBinItem->ids.rawAirSpellDamage = pItem->ids.rawAirSpellDamage;
        pBinItem->ids.rawWaterSpellDamage = pItem->ids.rawWaterSpellDamage;

        pBinItem->ids.healing = pItem->ids.healing;
        pBinItem->ids.rawHealth = pItem->ids.rawHealth;
        pBinItem->ids.healingEfficiency = pItem->ids.healingEfficiency;
        pBinItem->ids.healthRegen = pItem->ids.healthRegen;
        pBinItem->ids.healthRegenRaw = pItem->ids.healthRegenRaw;
        pBinItem->ids.lifeSteal = pItem->ids.lifeSteal;

        pBinItem->ids.manaRegen = pItem->ids.manaRegen;
        pBinItem->ids.manaSteal = pItem->ids.manaSteal;

        pBinItem->ids.sprint = pItem->ids.sprint;
        pBinItem->ids.sprintRegen = pItem->ids.sprintRegen;
        pBinItem->ids.walkSpeed = pItem->ids.walkSpeed;
        pBinItem->ids.jumpHeight = pItem->ids.jumpHeight;

        pBinItem->ids.thorns = pItem->ids.thorns;
        pBinItem->ids.reflection = pItem->ids.reflection;
        pBinItem->ids.knockback = pItem->ids.knockback;
        pBinItem->ids.exploding = pItem->ids.exploding;
        pBinItem->ids.poison = pItem->ids.poison;

        pBinItem->ids.stealing = pItem->ids.stealing;
        pBinItem->ids.xpBonus = pItem->ids.xpBonus;
        pBinItem->ids.lootBonus = pItem->ids.lootBonus;
        pBinItem->ids.soulPointRegen = pItem->ids.soulPointRegen; // May change in next update

        pBinItem->ids.SpellCost1st = pItem->ids.SpellCost1st;
        pBinItem->ids.SpellCost2nd = pItem->ids.SpellCost2nd;
        pBinItem->ids.SpellCost3rd = pItem->ids.SpellCost3rd;
        pBinItem->ids.SpellCost4th = pItem->ids.SpellCost4th;

        pBinItem->ids.rawSpellCost1st = pItem->ids.rawSpellCost1st;
        pBinItem->ids.rawSpellCost2nd = pItem->ids.rawSpellCost2nd;
        pBinItem->ids.rawSpellCost3rd = pItem->ids.rawSpellCost3rd;
        pBinItem->ids.rawSpellCost4th = pItem->ids.rawSpellCost4th;
        
        pBinItem->ids.slowEnemy = pItem->ids.slowEnemy;
        pBinItem->ids.weakenEnemy = pItem->ids.weakenEnemy;
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
        WynnItem item = {0};

        item.pName = wynnitem_name_pool_alloc(pNamePool);
        memcpy_s(item.pName->str, sizeof(item.pName->str), pBinItem->name, sizeof(pBinItem->name));
        
        item.type = pBinItem->type;
        item.tier = pBinItem->tier;
        item.powderSlots = pBinItem->powderSlots;
        item.attackSpeed = pBinItem->attackSpeed;

        item.reqs.class = pBinItem->reqs.class;
        item.reqs.level = pBinItem->reqs.level;
        item.reqs.strength = pBinItem->reqs.strength;
        item.reqs.dexterity = pBinItem->reqs.dexterity;
        item.reqs.defence = pBinItem->reqs.defence;
        item.reqs.agility = pBinItem->reqs.agility;
        item.reqs.intelligence = pBinItem->reqs.intelligence;

        item.base.averageDPS = pBinItem->base.averageDPS;
        item.base.damage = pBinItem->base.damage;
        item.base.earthDamage = pBinItem->base.earthDamage;
        item.base.thunderDamage = pBinItem->base.thunderDamage;
        item.base.fireDamage = pBinItem->base.fireDamage;
        item.base.airDamage = pBinItem->base.airDamage;
        item.base.waterDamage = pBinItem->base.waterDamage;

        item.base.health = pBinItem->base.health;
        item.base.earthDefence = pBinItem->base.earthDefence;
        item.base.thunderDefence = pBinItem->base.thunderDefence;
        item.base.fireDefence = pBinItem->base.fireDefence;
        item.base.airDefence = pBinItem->base.airDefence;
        item.base.waterDefence = pBinItem->base.waterDefence;

        item.ids.rawAttackSpeed = pBinItem->ids.rawAttackSpeed;
        item.ids.rawStrength = pBinItem->ids.rawStrength;
        item.ids.rawDexterity = pBinItem->ids.rawDexterity;
        item.ids.rawDefence = pBinItem->ids.rawDefence;
        item.ids.rawAgility = pBinItem->ids.rawAgility;
        item.ids.rawIntelligence = pBinItem->ids.rawIntelligence;

        item.ids.elementalDamage = pBinItem->ids.elementalDamage;
        item.ids.earthDamage = pBinItem->ids.earthDamage;
        item.ids.thunderDamage = pBinItem->ids.thunderDamage;
        item.ids.fireDamage = pBinItem->ids.fireDamage;
        item.ids.airDamage = pBinItem->ids.airDamage;
        item.ids.waterDamage = pBinItem->ids.waterDamage;

        item.ids.elementalDefence = pBinItem->ids.elementalDefence;
        item.ids.earthDefence = pBinItem->ids.earthDefence;
        item.ids.thunderDefence = pBinItem->ids.thunderDefence;
        item.ids.fireDefence = pBinItem->ids.fireDefence;
        item.ids.airDefence = pBinItem->ids.airDefence;
        item.ids.waterDefence = pBinItem->ids.waterDefence;

        item.ids.rawElementalDamage = pBinItem->ids.rawElementalDamage;
        item.ids.rawEarthDamage = pBinItem->ids.rawEarthDamage;
        item.ids.rawThunderDamage = pBinItem->ids.rawThunderDamage;
        item.ids.rawFireDamage = pBinItem->ids.rawFireDamage;
        item.ids.rawAirDamage = pBinItem->ids.rawAirDamage;
        item.ids.rawWaterDamage = pBinItem->ids.rawWaterDamage;

        item.ids.mainAttackDamage = pBinItem->ids.mainAttackDamage;
        item.ids.earthMainAttackDamage = pBinItem->ids.earthMainAttackDamage;
        item.ids.thunderMainAttackDamage = pBinItem->ids.thunderMainAttackDamage;
        item.ids.fireMainAttackDamage = pBinItem->ids.fireMainAttackDamage;
        item.ids.airMainAttackDamage = pBinItem->ids.airMainAttackDamage;
        item.ids.waterMainAttackDamage = pBinItem->ids.waterMainAttackDamage;

        item.ids.rawMainAttackDamage = pBinItem->ids.rawMainAttackDamage;
        item.ids.rawElementalMainAttackDamage = pBinItem->ids.rawElementalMainAttackDamage;
        item.ids.rawEarthMainAttackDamage = pBinItem->ids.rawEarthMainAttackDamage;
        item.ids.rawThunderMainAttackDamage = pBinItem->ids.rawThunderMainAttackDamage;
        item.ids.rawFireMainAttackDamage = pBinItem->ids.rawFireMainAttackDamage;
        item.ids.rawAirMainAttackDamage = pBinItem->ids.rawAirMainAttackDamage;
        item.ids.rawWaterMainAttackDamage = pBinItem->ids.rawWaterMainAttackDamage;

        item.ids.spellDamage = pBinItem->ids.spellDamage;
        item.ids.elementalSpellDamage = pBinItem->ids.elementalSpellDamage;
        item.ids.earthSpellDamage = pBinItem->ids.earthSpellDamage;
        item.ids.thunderSpellDamage = pBinItem->ids.thunderSpellDamage;
        item.ids.fireSpellDamage = pBinItem->ids.fireSpellDamage;
        item.ids.airSpellDamage = pBinItem->ids.airSpellDamage;
        item.ids.waterSpellDamage = pBinItem->ids.waterSpellDamage;

        item.ids.rawSpellDamage = pBinItem->ids.rawSpellDamage;
        item.ids.rawNeutralSpellDamage = pBinItem->ids.rawNeutralSpellDamage;
        item.ids.rawElementalSpellDamage = pBinItem->ids.rawElementalSpellDamage;
        item.ids.rawEarthSpellDamage = pBinItem->ids.rawEarthSpellDamage;
        item.ids.rawThunderSpellDamage = pBinItem->ids.rawThunderSpellDamage;
        item.ids.rawFireSpellDamage = pBinItem->ids.rawFireSpellDamage;
        item.ids.rawAirSpellDamage = pBinItem->ids.rawAirSpellDamage;
        item.ids.rawWaterSpellDamage = pBinItem->ids.rawWaterSpellDamage;

        item.ids.healing = pBinItem->ids.healing;
        item.ids.rawHealth = pBinItem->ids.rawHealth;
        item.ids.healingEfficiency = pBinItem->ids.healingEfficiency;
        item.ids.healthRegen = pBinItem->ids.healthRegen;
        item.ids.healthRegenRaw = pBinItem->ids.healthRegenRaw;
        item.ids.lifeSteal = pBinItem->ids.lifeSteal;

        item.ids.manaRegen = pBinItem->ids.manaRegen;
        item.ids.manaSteal = pBinItem->ids.manaSteal;

        item.ids.sprint = pBinItem->ids.sprint;
        item.ids.sprintRegen = pBinItem->ids.sprintRegen;
        item.ids.walkSpeed = pBinItem->ids.walkSpeed;
        item.ids.jumpHeight = pBinItem->ids.jumpHeight;

        item.ids.thorns = pBinItem->ids.thorns;
        item.ids.reflection = pBinItem->ids.reflection;
        item.ids.knockback = pBinItem->ids.knockback;
        item.ids.exploding = pBinItem->ids.exploding;
        item.ids.poison = pBinItem->ids.poison;

        item.ids.stealing = pBinItem->ids.stealing;
        item.ids.xpBonus = pBinItem->ids.xpBonus;
        item.ids.lootBonus = pBinItem->ids.lootBonus;
        item.ids.soulPointRegen = pBinItem->ids.soulPointRegen; // May change in next update

        item.ids.SpellCost1st = pBinItem->ids.SpellCost1st;
        item.ids.SpellCost2nd = pBinItem->ids.SpellCost2nd;
        item.ids.SpellCost3rd = pBinItem->ids.SpellCost3rd;
        item.ids.SpellCost4th = pBinItem->ids.SpellCost4th;

        item.ids.rawSpellCost1st = pBinItem->ids.rawSpellCost1st;
        item.ids.rawSpellCost2nd = pBinItem->ids.rawSpellCost2nd;
        item.ids.rawSpellCost3rd = pBinItem->ids.rawSpellCost3rd;
        item.ids.rawSpellCost4th = pBinItem->ids.rawSpellCost4th;
        
        item.ids.slowEnemy = pBinItem->ids.slowEnemy;
        item.ids.weakenEnemy = pBinItem->ids.weakenEnemy;

        WynnItem* pItem = wynnitem_pool_alloc(pItemPool);
        *pItem = item;
        wynnitem_list_append(&itemList, pItem);
    }

    return itemList;
}