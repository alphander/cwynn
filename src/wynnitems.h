#ifndef WYNNBUILD_H
#define WYNNBUILD_H

#include <LTK/containers.h>
#include <LTK/jsonparser.h>

#define lengthof(array) (sizeof(array) / sizeof(*array))

static const char* wynnItemReqsNames[] = {
    "level",
    "strength",
    "dexterity",
    "defence",
    "agility",
    "intelligence",
};

static const char* wynnItemBaseNames[] = {
    "averageDPS",
    "damage",
    "earthDamage",
    "thunderDamage",
    "fireDamage",
    "airDamage",
    "waterDamage",

    "health",
    "earthDefence",
    "thunderDefence",
    "fireDefence",
    "airDefence",
    "waterDefence",
};

static const char* wynnItemIdNames[] = {
    "rawAttackSpeed",

    "rawStrength",
    "rawDexterity",
    "rawDefence",
    "rawAgility",
    "rawIntelligence",

    "elementalDamage",
    "earthDamage",
    "thunderDamage",
    "fireDamage",
    "airDamage",
    "waterDamage",

    "elementalDefence",
    "earthDefence",
    "thunderDefence",
    "fireDefence",
    "airDefence",
    "waterDefence",

    "rawElementalDamage",
    "rawEarthDamage",
    "rawThunderDamage",
    "rawFireDamage",
    "rawAirDamage",
    "rawWaterDamage",

    "mainAttackDamage",
    "earthMainAttackDamage",
    "thunderMainAttackDamage",
    "fireMainAttackDamage",
    "airMainAttackDamage",
    "waterMainAttackDamage",

    "rawMainAttackDamage",
    "rawElementalMainAttackDamage",
    "rawEarthMainAttackDamage",
    "rawThunderMainAttackDamage",
    "rawFireMainAttackDamage",
    "rawAirMainAttackDamage",
    "rawWaterMainAttackDamage",

    "spellDamage",
    "elementalSpellDamage",
    "earthSpellDamage",
    "thunderSpellDamage",
    "fireSpellDamage",
    "airSpellDamage",
    "waterSpellDamage",

    "rawSpellDamage",
    "rawNeutralSpellDamage",
    "rawElementalSpellDamage",
    "rawEarthSpellDamage",
    "rawThunderSpellDamage",
    "rawFireSpellDamage",
    "rawAirSpellDamage",
    "rawWaterSpellDamage",

    "healing",
    "rawHealth",
    "healingEfficiency",
    "healthRegen",
    "healthRegenRaw",
    "lifeSteal",

    "manaRegen",
    "manaSteal",

    "sprint",
    "sprintRegen",
    "walkSpeed",
    "jumpHeight",

    "thorns",
    "reflection",
    "knockback",
    "exploding",
    "poison",

    "stealing",
    "xpBonus",
    "lootBonus",

    "soulPointRegen", // May change in next update

    "spellCost1st",
    "spellCost2nd",
    "spellCost3rd",
    "spellCost4th",

    "rawSpellCost1st",
    "rawSpellCost2nd",
    "rawSpellCost3rd",
    "rawSpellCost4th",

    "slowEnemy",
    "weakenEnemy",
};

typedef enum
{
    WYNNITEM_TYPE_HELMET = 0,
    WYNNITEM_TYPE_CHESTPLATE = 1,
    WYNNITEM_TYPE_LEGGINGS = 2,
    WYNNITEM_TYPE_BOOTS = 3,
    WYNNITEM_TYPE_RING = 4,
    WYNNITEM_TYPE_BRACELET = 5,
    WYNNITEM_TYPE_NECKLACE = 6,
    WYNNITEM_TYPE_WEAPON = 7,
} WynnItemType;

typedef enum
{
    WYNNITEM_CLASS_MAGE = 0,
    WYNNITEM_CLASS_WARRIOR = 1,
    WYNNITEM_CLASS_ARCHER = 2,
    WYNNITEM_CLASS_ASSASSIN = 3,
    WYNNITEM_CLASS_SHAMAN = 4,
} WynnItemClass;

typedef enum 
{
    WYNNITEM_TIER_COMMON = 0,
    WYNNITEM_TIER_UNIQUE = 1,
    WYNNITEM_TIER_RARE = 2,
    WYNNITEM_TIER_LEGENDARY = 3,
    WYNNITEM_TIER_FABLED = 4,
    WYNNITEM_TIER_MYTHIC = 5,
    WYNNITEM_TIER_SET = 6,
} WynnItemTier;

typedef enum 
{
    WYNNITEM_ATTACK_SPEED_SUPER_SLOW = 0,
    WYNNITEM_ATTACK_SPEED_VERY_SLOW = 1,
    WYNNITEM_ATTACK_SPEED_SLOW = 2,
    WYNNITEM_ATTACK_SPEED_NORMAL = 3,
    WYNNITEM_ATTACK_SPEED_FAST = 4,
    WYNNITEM_ATTACK_SPEED_VERY_FAST = 5,
    WYNNITEM_ATTACK_SPEED_SUPER_FAST = 6,
} WynnItemAttackSpeed;

typedef enum
{
    WYNNITEM_REQ_LEVEL = 0,
    WYNNITEM_REQ_STRENGTH = 1,
    WYNNITEM_REQ_DEXTERITY = 2,
    WYNNITEM_REQ_DEFENCE = 3,
    WYNNITEM_REQ_AGILITY = 4,
    WYNNITEM_REQ_INTELLIGENCE = 5,

    WYNNITEM_BASE_AVERAGE_DPS = 6,
    WYNNITEM_BASE_DAMAGE = 7,
    WYNNITEM_BASE_EARTH_DAMAGE = 8,
    WYNNITEM_BASE_THUNDER_DAMAGE = 9,
    WYNNITEM_BASE_FIRE_DAMAGE = 10,
    WYNNITEM_BASE_AIR_DAMAGE = 11,
    WYNNITEM_BASE_WATER_DAMAGE = 12,

    WYNNITEM_BASE_HEALTH = 13,
    WYNNITEM_BASE_EARTH_DEFENCE = 14,
    WYNNITEM_BASE_THUNDER_DEFENCE = 15,
    WYNNITEM_BASE_FIRE_DEFENCE = 16,
    WYNNITEM_BASE_AIR_DEFENCE = 17,
    WYNNITEM_BASE_WATER_DEFENCE = 18,

    WYNNITEM_ID_RAW_ATTACK_SPEED = 19,

    WYNNITEM_ID_RAW_STRENGTH = 20,
    WYNNITEM_ID_RAW_DEXTERITY = 21,
    WYNNITEM_ID_RAW_DEFENCE = 22,
    WYNNITEM_ID_RAW_AGILITY = 23,
    WYNNITEM_ID_RAW_INTELLIGENCE = 24,

    WYNNITEM_ID_ELEMENTAL_DAMAGE = 25,
    WYNNITEM_ID_EARTH_DAMAGE = 26,
    WYNNITEM_ID_THUNDER_DAMAGE = 27,
    WYNNITEM_ID_FIRE_DAMAGE = 28,
    WYNNITEM_ID_AIR_DAMAGE = 29,
    WYNNITEM_ID_WATER_DAMAGE = 30,

    WYNNITEM_ID_ELEMENTAL_DEFENCE = 31,
    WYNNITEM_ID_EARTH_DEFENCE = 32,
    WYNNITEM_ID_THUNDER_DEFENCE = 33,
    WYNNITEM_ID_FIRE_DEFENCE = 34,
    WYNNITEM_ID_AIR_DEFENCE = 35,
    WYNNITEM_ID_WATER_DEFENCE = 36,

    WYNNITEM_ID_RAW_ELEMENTAL_DAMAGE = 37,
    WYNNITEM_ID_RAW_EARTH_DAMAGE = 38,
    WYNNITEM_ID_RAW_THUNDER_DAMAGE = 39,
    WYNNITEM_ID_RAW_FIRE_DAMAGE = 40,
    WYNNITEM_ID_RAW_AIR_DAMAGE = 41,
    WYNNITEM_ID_RAW_WATER_DAMAGE = 42,

    WYNNITEM_ID_MAIN_ATTACK_DAMAGE = 43,
    WYNNITEM_ID_EARTH_MAIN_ATTACK_DAMAGE = 44,
    WYNNITEM_ID_THUNDER_MAIN_ATTACK_DAMAGE = 45,
    WYNNITEM_ID_FIRE_MAIN_ATTACK_DAMAGE = 46,
    WYNNITEM_ID_AIR_MAIN_ATTACK_DAMAGE = 47,
    WYNNITEM_ID_WATER_MAIN_ATTACK_DAMAGE = 48,

    WYNNITEM_ID_RAW_MAIN_ATTACK_DAMAGE = 49,
    WYNNITEM_ID_RAW_ELEMENTAL_MAIN_ATTACK_DAMAGE = 50,
    WYNNITEM_ID_RAW_EARTH_MAIN_ATTACK_DAMAGE = 51,
    WYNNITEM_ID_RAW_THUNDER_MAIN_ATTACK_DAMAGE = 52,
    WYNNITEM_ID_RAW_FIRE_MAIN_ATTACK_DAMAGE = 53,
    WYNNITEM_ID_RAW_AIR_MAIN_ATTACK_DAMAGE = 54,
    WYNNITEM_ID_RAW_WATER_MAIN_ATTACK_DAMAGE = 55,

    WYNNITEM_ID_SPELL_DAMAGE = 56,
    WYNNITEM_ID_ELEMENTAL_SPELL_DAMAGE = 57,
    WYNNITEM_ID_EARTH_SPELL_DAMAGE = 58,
    WYNNITEM_ID_THUNDER_SPELL_DAMAGE = 59,
    WYNNITEM_ID_FIRE_SPELL_DAMAGE = 60,
    WYNNITEM_ID_AIR_SPELL_DAMAGE = 61,
    WYNNITEM_ID_WATER_SPELL_DAMAGE = 62,

    WYNNITEM_ID_RAW_SPELL_DAMAGE = 63,
    WYNNITEM_ID_RAW_NEUTRAL_SPELL_DAMAGE = 64,
    WYNNITEM_ID_RAW_ELEMENTAL_SPELL_DAMAGE = 65,
    WYNNITEM_ID_RAW_EARTH_SPELL_DAMAGE = 66,
    WYNNITEM_ID_RAW_THUNDER_SPELL_DAMAGE = 67,
    WYNNITEM_ID_RAW_FIRE_SPELL_DAMAGE = 68,
    WYNNITEM_ID_RAW_AIR_SPELL_DAMAGE = 69,
    WYNNITEM_ID_RAW_WATER_SPELL_DAMAGE = 70,

    WYNNITEM_ID_HEALING = 71,
    WYNNITEM_ID_RAW_HEALTH = 72,
    WYNNITEM_ID_HEALING_EFFICIENCY = 73,
    WYNNITEM_ID_HEALTH_REGEN = 74,
    WYNNITEM_ID_HEALTH_REGEN_RAW = 75,
    WYNNITEM_ID_LIFE_STEAL = 76,

    WYNNITEM_ID_MANA_REGEN = 77,
    WYNNITEM_ID_MANA_STEAL = 78,

    WYNNITEM_ID_SPRINT = 79,
    WYNNITEM_ID_SPRINT_REGEN = 80,
    WYNNITEM_ID_WALK_SPEED = 81,
    WYNNITEM_ID_JUMP_HEIGHT = 82,

    WYNNITEM_ID_THORNS = 83,
    WYNNITEM_ID_REFLECTION = 84,
    WYNNITEM_ID_KNOCKBACK = 85,
    WYNNITEM_ID_EXPLODING = 86,
    WYNNITEM_ID_POISON = 87,

    WYNNITEM_ID_STEALING = 88,
    WYNNITEM_ID_XP_BONUS = 89,
    WYNNITEM_ID_LOOT_BONUS = 90,
    WYNNITEM_ID_SOUL_POINT_REGEN = 91,

    WYNNITEM_ID_SPELL_COST1ST = 92,
    WYNNITEM_ID_SPELL_COST2ND = 93,
    WYNNITEM_ID_SPELL_COST3RD = 94,
    WYNNITEM_ID_SPELL_COST4TH = 95,

    WYNNITEM_ID_RAW_SPELL_COST1ST = 96,
    WYNNITEM_ID_RAW_SPELL_COST2ND = 97,
    WYNNITEM_ID_RAW_SPELL_COST3RD = 98,
    WYNNITEM_ID_RAW_SPELL_COST4TH = 99,

    WYNNITEM_ID_SLOW_ENEMY = 100,
    WYNNITEM_ID_WEAKEN_ENEMY = 101,
} WynnItemIds;

// Keep under 128
typedef struct {char str[64];} WynnItemName;

struct wynnitem_reqs {
    int32_t level;
    int32_t strength;
    int32_t dexterity;
    int32_t defence;
    int32_t agility;
    int32_t intelligence;
};

struct wynnitem_base {
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
};

struct wynnitem_ids{
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
    int32_t soulPointRegen; // May change in next update

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
};

#define WYNNITEM_ID_ARRAY_SIZE\
    sizeof(struct wynnitem_reqs) / sizeof(int32_t) +\
    sizeof(struct wynnitem_base) / sizeof(int32_t) +\
    sizeof(struct wynnitem_ids) / sizeof(int32_t)
typedef int32_t WynnItemIdArray[WYNNITEM_ID_ARRAY_SIZE];

typedef struct {
    WynnItemName* pName;
    WynnItemType type;
    WynnItemTier tier;
    WynnItemClass class;
    uint8_t powderSlots;
    int32_t attackSpeed;
    
    // Requirement
    union {
        WynnItemIdArray idArray;        
        struct {
            struct wynnitem_reqs reqs;
            struct wynnitem_base base;
            struct wynnitem_ids ids;
        };
    };
} WynnItem;

POOL_GENERIC_EX(WynnItem, WynnItemPool, wynnitem_pool)
POOL_GENERIC_EX(WynnItemName, WynnItemNamePool, wynnitem_name_pool)
LIST_GENERIC_EX(WynnItem*, WynnItemList, wynnitem_list)

typedef struct
{
    union {
        WynnItem* pItems[9];
        struct {
            WynnItem* pHelmet;
            WynnItem* pChestplate;
            WynnItem* pLeggings;
            WynnItem* pBoots;
            WynnItem* pRing1;
            WynnItem* pRing2;
            WynnItem* pBracelet;
            WynnItem* pNecklace;
            WynnItem* pWeapon;
        };
    };
} WynnBuild;

float wynnitem_similarity(WynnItem* pItem, WynnItem* pTestItem);
float wynnitem_get_value(size_t index);
void wynnitem_set_value(size_t index, float value);

void wynnitems_init(WynnItemList* pItemList);
void wynnitems_cleanup();
WynnBuild wynnitems_calculate_build(size_t numIters);
#endif // WYNNBUILD_H