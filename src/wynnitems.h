#ifndef WYNNBUILD_H
#define WYNNBUILD_H

#include "LTK/containers.h"
#include "LTK/jsonparser.h"

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

// Keep under 128
typedef struct {char str[64];} WynnItemName;

typedef struct {
    WynnItemName* pName;
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
    } ids;
} WynnItem;

POOL_GENERIC_EX(WynnItem, WynnItemPool, wynnitem_pool)
POOL_GENERIC_EX(WynnItemName, WynnItemNamePool, wynnitem_name_pool)
LIST_GENERIC_EX(WynnItem*, WynnItemList, wynnitem_list)

typedef struct
{
    WynnItem* pHelmet;
    WynnItem* pChestplate;
    WynnItem* pLeggings;
    WynnItem* pBoots;
    WynnItem* pRing1;
    WynnItem* pRing2;
    WynnItem* pBracelet;
    WynnItem* pNecklace;
    WynnItem* pWeapon;
} WynnBuild;

float wynnitem_similarity(WynnItem* pItem, WynnItem* pTestItem);

#endif // WYNNBUILD_H