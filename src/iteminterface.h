#ifndef ITEMINTERFACE_H
#define ITEMINTERFACE_H

typedef struct
{
    uint8_t powderSlots;
    int32_t attackSpeed;
    
    // Requirement
    struct {
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
} ItemPreferences;

int iteminterface_run(void* pArgs);

#endif // ITEMINTERFACE_H