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

void wynnitem_unload()
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
        WynnItem* pItem = wynnitem_pool_alloc(pItemPool);
        JsonValue* pTypeStr = NULL;
        if ((pTypeStr = json_object_get(pJsonItem, (JsonKey){"type"})) ||
            (pTypeStr = json_object_get(pJsonItem, (JsonKey){"accessoryType"})))
        {
            const char* type = json_to_string(pTypeStr);
            if (!strcmp(type, "helmet")) pItem->type = WYNNITEM_TYPE_HELMET;
            else if (!strcmp(type, "chestplate")) pItem->type = WYNNITEM_TYPE_CHESTPLATE;
            else if (!strcmp(type, "leggings")) pItem->type = WYNNITEM_TYPE_LEGGINGS;
            else if (!strcmp(type, "boots")) pItem->type = WYNNITEM_TYPE_BOOTS;
            else if (!strcmp(type, "ring")) pItem->type = WYNNITEM_TYPE_RING;
            else if (!strcmp(type, "bracelet")) pItem->type = WYNNITEM_TYPE_BRACELET;
            else if (!strcmp(type, "necklace")) pItem->type = WYNNITEM_TYPE_NECKLACE;
            else if (!strcmp(type, "spear")) pItem->type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "bow")) pItem->type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "wand")) pItem->type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "dagger")) pItem->type = WYNNITEM_TYPE_WEAPON;
            else if (!strcmp(type, "relik")) pItem->type = WYNNITEM_TYPE_WEAPON;
            else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
        } 
        else continue;

        JsonValue* pTierStr = json_object_get(pJsonItem, (JsonKey){"tier"});
        const char* tier = json_to_string(pTierStr);
        if (!strcmp(tier, "common")) pItem->tier = WYNNITEM_TIER_COMMON;
        else if (!strcmp(tier, "unique")) pItem->tier = WYNNITEM_TIER_UNIQUE;
        else if (!strcmp(tier, "rare")) pItem->tier = WYNNITEM_TIER_RARE;
        else if (!strcmp(tier, "legendary")) pItem->tier = WYNNITEM_TIER_LEGENDARY;
        else if (!strcmp(tier, "fabled")) pItem->tier = WYNNITEM_TIER_FABLED;
        else if (!strcmp(tier, "mythic")) pItem->tier = WYNNITEM_TIER_MYTHIC;
        else if (!strcmp(tier, "set")) pItem->tier = WYNNITEM_TIER_SET;
        else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});

        JsonValue* pAttackSpeedStr = json_object_get(pJsonItem, (JsonKey){"attackSpeed"});
        if (pAttackSpeedStr != NULL)
        {
            const char* speed = json_to_string(pAttackSpeedStr);
            if (!strcmp(speed, "super_slow")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_SUPER_SLOW;
            else if (!strcmp(speed, "very_slow")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_VERY_SLOW;
            else if (!strcmp(speed, "slow")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_SLOW;
            else if (!strcmp(speed, "normal")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_NORMAL;
            else if (!strcmp(speed, "fast")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_FAST;
            else if (!strcmp(speed, "very_fast")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_VERY_FAST;
            else if (!strcmp(speed, "super_fast")) pItem->attackSpeed = WYNNITEM_ATTACK_SPEED_SUPER_FAST;
            else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
        }

        JsonValue* pReqsSection = json_object_get(pJsonItem, (JsonKey){"requirements"});
        if (pReqsSection != NULL)
        {
            JsonValue* pClassReq = json_object_get(pJsonItem, (JsonKey){"classRequirement"});
            if (pClassReq != NULL)
            {
                const char* class = json_to_string(pClassReq);
                if (!strcmp(class, "mage")) pItem->reqs.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "warrior")) pItem->reqs.class = WYNNITEM_CLASS_WARRIOR;
                else if (!strcmp(class, "archer")) pItem->reqs.class = WYNNITEM_CLASS_ARCHER;
                else if (!strcmp(class, "assassin")) pItem->reqs.class = WYNNITEM_CLASS_ASSASSIN;
                else if (!strcmp(class, "shaman")) pItem->reqs.class = WYNNITEM_CLASS_SHAMAN;
                else ERR_RET(true, ERR_FAILURE, (WynnItemList){0});
            }

            pItem->reqs.level = min_max_entry(pReqsSection, "level");
            pItem->reqs.strength = min_max_entry(pReqsSection, "strength");
            pItem->reqs.dexterity = min_max_entry(pReqsSection, "dexterity");
            pItem->reqs.defence = min_max_entry(pReqsSection, "defence");
            pItem->reqs.agility = min_max_entry(pReqsSection, "agility");
            pItem->reqs.intelligence = min_max_entry(pReqsSection, "intelligence");
        }

        JsonValue* pBaseSection = json_object_get(pJsonItem, (JsonKey){"base"});
        if (pBaseSection != NULL)
        {
            pItem->base.averageDPS = min_max_entry(pBaseSection, "averageDPS");
            pItem->base.damage = min_max_entry(pBaseSection, "damage");
            pItem->base.earthDamage = min_max_entry(pBaseSection, "earthDamage");
            pItem->base.thunderDamage = min_max_entry(pBaseSection, "thunderDamage");
            pItem->base.fireDamage = min_max_entry(pBaseSection, "fireDamage");
            pItem->base.airDamage = min_max_entry(pBaseSection, "airDamage");
            pItem->base.waterDamage = min_max_entry(pBaseSection, "waterDamage");

            pItem->base.health = min_max_entry(pBaseSection, "health");
            pItem->base.earthDefence = min_max_entry(pBaseSection, "earthDefence");
            pItem->base.thunderDefence = min_max_entry(pBaseSection, "thunderDefence");
            pItem->base.fireDefence = min_max_entry(pBaseSection, "fireDefence");
            pItem->base.airDefence = min_max_entry(pBaseSection, "airDefence");
            pItem->base.waterDefence = min_max_entry(pBaseSection, "waterDefence");
        }

        JsonValue* pIdsSection = json_object_get(pJsonItem, (JsonKey){"identifications"});
        if (pIdsSection != NULL)
        {
            pItem->ids.rawAttackSpeed = min_max_entry(pIdsSection, "rawAttackSpeed");

            pItem->ids.rawStrength = min_max_entry(pIdsSection, "rawStrength");
            pItem->ids.rawDexterity = min_max_entry(pIdsSection, "rawDexterity");
            pItem->ids.rawDefence = min_max_entry(pIdsSection, "rawDefence");
            pItem->ids.rawAgility = min_max_entry(pIdsSection, "rawAgility");
            pItem->ids.rawIntelligence = min_max_entry(pIdsSection, "rawIntelligence");

            pItem->ids.elementalDamage = min_max_entry(pIdsSection, "elementalDamage");
            pItem->ids.earthDamage = min_max_entry(pIdsSection, "earthDamage");
            pItem->ids.thunderDamage = min_max_entry(pIdsSection, "thunderDamage");
            pItem->ids.fireDamage = min_max_entry(pIdsSection, "fireDamage");
            pItem->ids.airDamage = min_max_entry(pIdsSection, "airDamage");
            pItem->ids.waterDamage = min_max_entry(pIdsSection, "waterDamage");

            pItem->ids.elementalDefence = min_max_entry(pIdsSection, "elementalDefence");
            pItem->ids.earthDefence = min_max_entry(pIdsSection, "earthDefence");
            pItem->ids.airDefence = min_max_entry(pIdsSection, "airDefence");
            pItem->ids.fireDefence = min_max_entry(pIdsSection, "fireDefence");
            pItem->ids.thunderDefence = min_max_entry(pIdsSection, "thunderDefence");
            pItem->ids.waterDefence = min_max_entry(pIdsSection, "waterDefence");

            pItem->ids.rawElementalDamage = min_max_entry(pIdsSection, "rawElementalDamage");
            pItem->ids.rawEarthDamage = min_max_entry(pIdsSection, "rawEarthDamage");
            pItem->ids.rawThunderDamage = min_max_entry(pIdsSection, "rawThunderDamage");
            pItem->ids.rawFireDamage = min_max_entry(pIdsSection, "rawFireDamage");
            pItem->ids.rawAirDamage = min_max_entry(pIdsSection, "rawAirDamage");
            pItem->ids.rawWaterDamage = min_max_entry(pIdsSection, "rawWaterDamage");

            pItem->ids.mainAttackDamage = min_max_entry(pIdsSection, "mainAttackDamage");
            pItem->ids.earthMainAttackDamage = min_max_entry(pIdsSection, "earthMainAttackDamage");
            pItem->ids.thunderMainAttackDamage = min_max_entry(pIdsSection, "thunderMainAttackDamage");
            pItem->ids.fireMainAttackDamage = min_max_entry(pIdsSection, "fireMainAttackDamage");
            pItem->ids.airMainAttackDamage = min_max_entry(pIdsSection, "airMainAttackDamage");
            pItem->ids.waterMainAttackDamage = min_max_entry(pIdsSection, "waterMainAttackDamage");

            pItem->ids.rawMainAttackDamage = min_max_entry(pIdsSection, "rawMainAttackDamage");
            pItem->ids.rawElementalMainAttackDamage = min_max_entry(pIdsSection, "rawElementalMainAttackDamage");
            pItem->ids.rawEarthMainAttackDamage = min_max_entry(pIdsSection, "rawEarthMainAttackDamage");
            pItem->ids.rawThunderMainAttackDamage = min_max_entry(pIdsSection, "rawThunderMainAttackDamage");
            pItem->ids.rawFireMainAttackDamage = min_max_entry(pIdsSection, "rawFireMainAttackDamage");
            pItem->ids.rawAirMainAttackDamage = min_max_entry(pIdsSection, "rawAirMainAttackDamage");
            pItem->ids.rawWaterMainAttackDamage = min_max_entry(pIdsSection, "rawWaterMainAttackDamage");

            pItem->ids.spellDamage = min_max_entry(pIdsSection, "spellDamage");
            pItem->ids.elementalSpellDamage = min_max_entry(pIdsSection, "elementalSpellDamage");
            pItem->ids.fireSpellDamage = min_max_entry(pIdsSection, "fireSpellDamage");
            pItem->ids.waterSpellDamage = min_max_entry(pIdsSection, "waterSpellDamage");
            pItem->ids.earthSpellDamage = min_max_entry(pIdsSection, "earthSpellDamage");
            pItem->ids.thunderSpellDamage = min_max_entry(pIdsSection, "thunderSpellDamage");
            pItem->ids.airSpellDamage = min_max_entry(pIdsSection, "airSpellDamage");

            pItem->ids.rawSpellDamage = min_max_entry(pIdsSection, "rawSpellDamage");
            pItem->ids.rawNeutralSpellDamage = min_max_entry(pIdsSection, "rawNeutralSpellDamage");
            pItem->ids.rawElementalSpellDamage = min_max_entry(pIdsSection, "rawElementalSpellDamage");
            pItem->ids.rawEarthSpellDamage = min_max_entry(pIdsSection, "rawEarthSpellDamage");
            pItem->ids.rawThunderSpellDamage = min_max_entry(pIdsSection, "rawThunderSpellDamage");
            pItem->ids.rawFireSpellDamage = min_max_entry(pIdsSection, "rawFireSpellDamage");
            pItem->ids.rawAirSpellDamage = min_max_entry(pIdsSection, "rawAirSpellDamage");
            pItem->ids.rawWaterSpellDamage = min_max_entry(pIdsSection, "rawWaterSpellDamage");

            pItem->ids.healing = min_max_entry(pIdsSection, "healing");
            pItem->ids.rawHealth = min_max_entry(pIdsSection, "rawHealth");
            pItem->ids.healingEfficiency = min_max_entry(pIdsSection, "healingEfficiency");
            pItem->ids.healthRegen = min_max_entry(pIdsSection, "healthRegen");
            pItem->ids.healthRegenRaw = min_max_entry(pIdsSection, "healthRegenRaw");
            pItem->ids.lifeSteal = min_max_entry(pIdsSection, "lifeSteal");

            pItem->ids.manaRegen = min_max_entry(pIdsSection, "manaRegen");
            pItem->ids.manaSteal = min_max_entry(pIdsSection, "manaSteal");

            pItem->ids.sprint = min_max_entry(pIdsSection, "sprint");
            pItem->ids.sprintRegen = min_max_entry(pIdsSection, "sprintRegen");
            pItem->ids.walkSpeed = min_max_entry(pIdsSection, "walkSpeed");
            pItem->ids.jumpHeight = min_max_entry(pIdsSection, "jumpHeight");

            pItem->ids.thorns = min_max_entry(pIdsSection, "thorns");
            pItem->ids.reflection = min_max_entry(pIdsSection, "reflection");
            pItem->ids.knockback = min_max_entry(pIdsSection, "knockback");
            pItem->ids.exploding = min_max_entry(pIdsSection, "exploding");
            pItem->ids.poison = min_max_entry(pIdsSection, "poison");

            pItem->ids.stealing = min_max_entry(pIdsSection, "stealing");
            pItem->ids.xpBonus = min_max_entry(pIdsSection, "xpBonus");
            pItem->ids.lootBonus = min_max_entry(pIdsSection, "lootBonus");
            pItem->ids.soulPointRegen = min_max_entry(pIdsSection, "soulPointRegen");

            pItem->ids.SpellCost1st = min_max_entry(pIdsSection, "1stSpellCost");
            pItem->ids.SpellCost2nd = min_max_entry(pIdsSection, "2ndSpellCost");
            pItem->ids.SpellCost3rd = min_max_entry(pIdsSection, "3rdSpellCost");
            pItem->ids.SpellCost4th = min_max_entry(pIdsSection, "4thSpellCost");

            pItem->ids.rawSpellCost1st = min_max_entry(pIdsSection, "raw1stSpellCost");
            pItem->ids.rawSpellCost2nd = min_max_entry(pIdsSection, "raw2ndSpellCost");
            pItem->ids.rawSpellCost3rd = min_max_entry(pIdsSection, "raw3rdSpellCost");
            pItem->ids.rawSpellCost4th = min_max_entry(pIdsSection, "raw4thSpellCost");

            pItem->ids.slowEnemy = min_max_entry(pIdsSection, "slowEnemy");
            pItem->ids.weakenEnemy = min_max_entry(pIdsSection, "weakenEnemy");
        }

        pItem->pName = wynnitem_name_pool_alloc(pNamePool);
        memset(pItem->pName->str, 0, sizeof(pItem->pName->str));
        strncpy_s(pItem->pName->str, sizeof(pItem->pName->str), itemName.str, sizeof(itemName.str));

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

        WynnItem* pItem = wynnitem_pool_alloc(pItemPool);
        wynnitem_list_append(&itemList, pItem);

        pItem->pName = wynnitem_name_pool_alloc(pNamePool);
        memcpy_s(pItem->pName->str, sizeof(pItem->pName->str), pBinItem->name, sizeof(pBinItem->name));
        
        pItem->type = pBinItem->type;
        pItem->tier = pBinItem->tier;
        pItem->powderSlots = pBinItem->powderSlots;
        pItem->attackSpeed = pBinItem->attackSpeed;

        pItem->reqs.class = pBinItem->reqs.class;
        pItem->reqs.level = pBinItem->reqs.level;
        pItem->reqs.strength = pBinItem->reqs.strength;
        pItem->reqs.dexterity = pBinItem->reqs.dexterity;
        pItem->reqs.defence = pBinItem->reqs.defence;
        pItem->reqs.agility = pBinItem->reqs.agility;
        pItem->reqs.intelligence = pBinItem->reqs.intelligence;

        pItem->base.averageDPS = pBinItem->base.averageDPS;
        pItem->base.damage = pBinItem->base.damage;
        pItem->base.earthDamage = pBinItem->base.earthDamage;
        pItem->base.thunderDamage = pBinItem->base.thunderDamage;
        pItem->base.fireDamage = pBinItem->base.fireDamage;
        pItem->base.airDamage = pBinItem->base.airDamage;
        pItem->base.waterDamage = pBinItem->base.waterDamage;

        pItem->base.health = pBinItem->base.health;
        pItem->base.earthDefence = pBinItem->base.earthDefence;
        pItem->base.thunderDefence = pBinItem->base.thunderDefence;
        pItem->base.fireDefence = pBinItem->base.fireDefence;
        pItem->base.airDefence = pBinItem->base.airDefence;
        pItem->base.waterDefence = pBinItem->base.waterDefence;

        pItem->ids.rawAttackSpeed = pBinItem->ids.rawAttackSpeed;
        pItem->ids.rawStrength = pBinItem->ids.rawStrength;
        pItem->ids.rawDexterity = pBinItem->ids.rawDexterity;
        pItem->ids.rawDefence = pBinItem->ids.rawDefence;
        pItem->ids.rawAgility = pBinItem->ids.rawAgility;
        pItem->ids.rawIntelligence = pBinItem->ids.rawIntelligence;

        pItem->ids.elementalDamage = pBinItem->ids.elementalDamage;
        pItem->ids.earthDamage = pBinItem->ids.earthDamage;
        pItem->ids.thunderDamage = pBinItem->ids.thunderDamage;
        pItem->ids.fireDamage = pBinItem->ids.fireDamage;
        pItem->ids.airDamage = pBinItem->ids.airDamage;
        pItem->ids.waterDamage = pBinItem->ids.waterDamage;

        pItem->ids.elementalDefence = pBinItem->ids.elementalDefence;
        pItem->ids.earthDefence = pBinItem->ids.earthDefence;
        pItem->ids.thunderDefence = pBinItem->ids.thunderDefence;
        pItem->ids.fireDefence = pBinItem->ids.fireDefence;
        pItem->ids.airDefence = pBinItem->ids.airDefence;
        pItem->ids.waterDefence = pBinItem->ids.waterDefence;

        pItem->ids.rawElementalDamage = pBinItem->ids.rawElementalDamage;
        pItem->ids.rawEarthDamage = pBinItem->ids.rawEarthDamage;
        pItem->ids.rawThunderDamage = pBinItem->ids.rawThunderDamage;
        pItem->ids.rawFireDamage = pBinItem->ids.rawFireDamage;
        pItem->ids.rawAirDamage = pBinItem->ids.rawAirDamage;
        pItem->ids.rawWaterDamage = pBinItem->ids.rawWaterDamage;

        pItem->ids.mainAttackDamage = pBinItem->ids.mainAttackDamage;
        pItem->ids.earthMainAttackDamage = pBinItem->ids.earthMainAttackDamage;
        pItem->ids.thunderMainAttackDamage = pBinItem->ids.thunderMainAttackDamage;
        pItem->ids.fireMainAttackDamage = pBinItem->ids.fireMainAttackDamage;
        pItem->ids.airMainAttackDamage = pBinItem->ids.airMainAttackDamage;
        pItem->ids.waterMainAttackDamage = pBinItem->ids.waterMainAttackDamage;

        pItem->ids.rawMainAttackDamage = pBinItem->ids.rawMainAttackDamage;
        pItem->ids.rawElementalMainAttackDamage = pBinItem->ids.rawElementalMainAttackDamage;
        pItem->ids.rawEarthMainAttackDamage = pBinItem->ids.rawEarthMainAttackDamage;
        pItem->ids.rawThunderMainAttackDamage = pBinItem->ids.rawThunderMainAttackDamage;
        pItem->ids.rawFireMainAttackDamage = pBinItem->ids.rawFireMainAttackDamage;
        pItem->ids.rawAirMainAttackDamage = pBinItem->ids.rawAirMainAttackDamage;
        pItem->ids.rawWaterMainAttackDamage = pBinItem->ids.rawWaterMainAttackDamage;

        pItem->ids.spellDamage = pBinItem->ids.spellDamage;
        pItem->ids.elementalSpellDamage = pBinItem->ids.elementalSpellDamage;
        pItem->ids.earthSpellDamage = pBinItem->ids.earthSpellDamage;
        pItem->ids.thunderSpellDamage = pBinItem->ids.thunderSpellDamage;
        pItem->ids.fireSpellDamage = pBinItem->ids.fireSpellDamage;
        pItem->ids.airSpellDamage = pBinItem->ids.airSpellDamage;
        pItem->ids.waterSpellDamage = pBinItem->ids.waterSpellDamage;

        pItem->ids.rawSpellDamage = pBinItem->ids.rawSpellDamage;
        pItem->ids.rawNeutralSpellDamage = pBinItem->ids.rawNeutralSpellDamage;
        pItem->ids.rawElementalSpellDamage = pBinItem->ids.rawElementalSpellDamage;
        pItem->ids.rawEarthSpellDamage = pBinItem->ids.rawEarthSpellDamage;
        pItem->ids.rawThunderSpellDamage = pBinItem->ids.rawThunderSpellDamage;
        pItem->ids.rawFireSpellDamage = pBinItem->ids.rawFireSpellDamage;
        pItem->ids.rawAirSpellDamage = pBinItem->ids.rawAirSpellDamage;
        pItem->ids.rawWaterSpellDamage = pBinItem->ids.rawWaterSpellDamage;

        pItem->ids.healing = pBinItem->ids.healing;
        pItem->ids.rawHealth = pBinItem->ids.rawHealth;
        pItem->ids.healingEfficiency = pBinItem->ids.healingEfficiency;
        pItem->ids.healthRegen = pBinItem->ids.healthRegen;
        pItem->ids.healthRegenRaw = pBinItem->ids.healthRegenRaw;
        pItem->ids.lifeSteal = pBinItem->ids.lifeSteal;

        pItem->ids.manaRegen = pBinItem->ids.manaRegen;
        pItem->ids.manaSteal = pBinItem->ids.manaSteal;

        pItem->ids.sprint = pBinItem->ids.sprint;
        pItem->ids.sprintRegen = pBinItem->ids.sprintRegen;
        pItem->ids.walkSpeed = pBinItem->ids.walkSpeed;
        pItem->ids.jumpHeight = pBinItem->ids.jumpHeight;

        pItem->ids.thorns = pBinItem->ids.thorns;
        pItem->ids.reflection = pBinItem->ids.reflection;
        pItem->ids.knockback = pBinItem->ids.knockback;
        pItem->ids.exploding = pBinItem->ids.exploding;
        pItem->ids.poison = pBinItem->ids.poison;

        pItem->ids.stealing = pBinItem->ids.stealing;
        pItem->ids.xpBonus = pBinItem->ids.xpBonus;
        pItem->ids.lootBonus = pBinItem->ids.lootBonus;
        pItem->ids.soulPointRegen = pBinItem->ids.soulPointRegen; // May change in next update

        pItem->ids.SpellCost1st = pBinItem->ids.SpellCost1st;
        pItem->ids.SpellCost2nd = pBinItem->ids.SpellCost2nd;
        pItem->ids.SpellCost3rd = pBinItem->ids.SpellCost3rd;
        pItem->ids.SpellCost4th = pBinItem->ids.SpellCost4th;

        pItem->ids.rawSpellCost1st = pBinItem->ids.rawSpellCost1st;
        pItem->ids.rawSpellCost2nd = pBinItem->ids.rawSpellCost2nd;
        pItem->ids.rawSpellCost3rd = pBinItem->ids.rawSpellCost3rd;
        pItem->ids.rawSpellCost4th = pBinItem->ids.rawSpellCost4th;
        
        pItem->ids.slowEnemy = pBinItem->ids.slowEnemy;
        pItem->ids.weakenEnemy = pBinItem->ids.weakenEnemy;
    }

    return itemList;
}