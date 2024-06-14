#ifndef INFO_UTILS_H
#define INFO_UTILS_H

#include "itemloader.h"

struct cmp_item {
    ssize_t name;
    ssize_t type;
    ssize_t tier;
    ssize_t powderSlots;
    ssize_t attackSpeed;
    
    // Requirement
    struct {
        ssize_t class;
        ssize_t level;
        ssize_t strength;
        ssize_t dexterity;
        ssize_t defence;
        ssize_t agility;
        ssize_t intelligence;
    } reqs;

    // Base
    struct {
        ssize_t averageDPS;
        ssize_t damage;
        ssize_t earthDamage;
        ssize_t thunderDamage;
        ssize_t fireDamage;
        ssize_t airDamage;
        ssize_t waterDamage;

        ssize_t health;
        ssize_t earthDefence;
        ssize_t thunderDefence;
        ssize_t fireDefence;
        ssize_t airDefence;
        ssize_t waterDefence;
    } base;

    // Identifications

    struct {
        ssize_t rawAttackSpeed;

        ssize_t rawStrength;
        ssize_t rawDexterity;
        ssize_t rawDefence;
        ssize_t rawAgility;
        ssize_t rawIntelligence;

        ssize_t elementalDamage;
        ssize_t earthDamage;
        ssize_t thunderDamage;
        ssize_t fireDamage;
        ssize_t airDamage;
        ssize_t waterDamage;

        ssize_t elementalDefence;
        ssize_t earthDefence;
        ssize_t thunderDefence;
        ssize_t fireDefence;
        ssize_t airDefence;
        ssize_t waterDefence;

        ssize_t rawElementalDamage;
        ssize_t rawEarthDamage;
        ssize_t rawThunderDamage;
        ssize_t rawFireDamage;
        ssize_t rawAirDamage;
        ssize_t rawWaterDamage;

        ssize_t mainAttackDamage;
        ssize_t earthMainAttackDamage;
        ssize_t thunderMainAttackDamage;
        ssize_t fireMainAttackDamage;
        ssize_t airMainAttackDamage;
        ssize_t waterMainAttackDamage;

        ssize_t rawMainAttackDamage;
        ssize_t rawElementalMainAttackDamage;
        ssize_t rawEarthMainAttackDamage;
        ssize_t rawThunderMainAttackDamage;
        ssize_t rawFireMainAttackDamage;
        ssize_t rawAirMainAttackDamage;
        ssize_t rawWaterMainAttackDamage;

        ssize_t spellDamage;
        ssize_t elementalSpellDamage;
        ssize_t earthSpellDamage;
        ssize_t thunderSpellDamage;
        ssize_t fireSpellDamage;
        ssize_t airSpellDamage;
        ssize_t waterSpellDamage;

        ssize_t rawSpellDamage;
        ssize_t rawNeutralSpellDamage;
        ssize_t rawElementalSpellDamage;
        ssize_t rawEarthSpellDamage;
        ssize_t rawThunderSpellDamage;
        ssize_t rawFireSpellDamage;
        ssize_t rawAirSpellDamage;
        ssize_t rawWaterSpellDamage;

        ssize_t healing;
        ssize_t rawHealth;
        ssize_t healingEfficiency;
        ssize_t healthRegen;
        ssize_t healthRegenRaw;
        ssize_t lifeSteal;

        ssize_t manaRegen;
        ssize_t manaSteal;

        ssize_t sprint;
        ssize_t sprintRegen;
        ssize_t walkSpeed;
        ssize_t jumpHeight;

        ssize_t thorns;
        ssize_t reflection;
        ssize_t knockback;
        ssize_t exploding;
        ssize_t poison;

        ssize_t stealing;
        ssize_t xpBonus;
        ssize_t lootBonus;
        ssize_t soulPointRegen; // May change in next update

        ssize_t SpellCost1st;
        ssize_t SpellCost2nd;
        ssize_t SpellCost3rd;
        ssize_t SpellCost4th;

        ssize_t rawSpellCost1st;
        ssize_t rawSpellCost2nd;
        ssize_t rawSpellCost3rd;
        ssize_t rawSpellCost4th;

        ssize_t slowEnemy;
        ssize_t weakenEnemy;
    } ids;
};

static inline ssize_t maxc(ssize_t a, ssize_t b)
{
    if (a > INT16_MAX ||b > INT16_MAX)
    {
        printf("test\n");
    }

    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    return a > b ? a : b;
}

static inline void print_recommendation(ssize_t v, const char* str)
{
    if (v > INT32_MAX)
        printf("(int64_t) %s\n", str);
    else if (v > INT16_MAX)
        printf("(int32_t) %s\n", str);
    else if (v > INT8_MAX)
        printf("(int16_t) %s\n", str);
    else
        printf("(int8_t) %s\n", str);
}

static void recommend_structure_bytes(WynnItemList* pItemList)
{
    struct cmp_item c = {0};

    for (size_t i = 0; i < wynnitem_list_size(pItemList); i++)
    {
        WynnItem* pI = wynnitem_list_get(pItemList, i);
        
        c.type = maxc(c.type, pI->type);
        c.tier = maxc(c.tier, pI->tier);
        c.powderSlots = maxc(c.powderSlots, pI->powderSlots);
        c.attackSpeed = maxc(c.attackSpeed, pI->attackSpeed);

        c.reqs.class = maxc(c.reqs.class, pI->reqs.class);
        c.reqs.level = maxc(c.reqs.level, pI->reqs.level);
        c.reqs.strength = maxc(c.reqs.strength, pI->reqs.strength);
        c.reqs.dexterity = maxc(c.reqs.dexterity, pI->reqs.dexterity);
        c.reqs.defence = maxc(c.reqs.defence, pI->reqs.defence);
        c.reqs.agility = maxc(c.reqs.agility, pI->reqs.agility);
        c.reqs.intelligence = maxc(c.reqs.intelligence, pI->reqs.intelligence);

        c.base.averageDPS = maxc(c.base.averageDPS, pI->base.averageDPS);
        c.base.damage = maxc(c.base.damage, pI->base.damage);
        c.base.earthDamage = maxc(c.base.earthDamage, pI->base.earthDamage);
        c.base.thunderDamage = maxc(c.base.thunderDamage, pI->base.thunderDamage);
        c.base.fireDamage = maxc(c.base.fireDamage, pI->base.fireDamage);
        c.base.airDamage = maxc(c.base.airDamage, pI->base.airDamage);
        c.base.waterDamage = maxc(c.base.waterDamage, pI->base.waterDamage);

        c.base.health = maxc(c.base.health, pI->base.health);
        c.base.earthDefence = maxc(c.base.earthDefence, pI->base.earthDefence);
        c.base.thunderDefence = maxc(c.base.thunderDefence, pI->base.thunderDefence);
        c.base.fireDefence = maxc(c.base.fireDefence, pI->base.fireDefence);
        c.base.airDefence = maxc(c.base.airDefence, pI->base.airDefence);
        c.base.waterDefence = maxc(c.base.waterDefence, pI->base.waterDefence);

        c.ids.rawAttackSpeed = maxc(c.ids.rawAttackSpeed, pI->ids.rawAttackSpeed);
        c.ids.rawStrength = maxc(c.ids.rawStrength, pI->ids.rawStrength);
        c.ids.rawDexterity = maxc(c.ids.rawDexterity, pI->ids.rawDexterity);
        c.ids.rawDefence = maxc(c.ids.rawDefence, pI->ids.rawDefence);
        c.ids.rawAgility = maxc(c.ids.rawAgility, pI->ids.rawAgility);
        c.ids.rawIntelligence = maxc(c.ids.rawIntelligence, pI->ids.rawIntelligence);

        c.ids.elementalDamage = maxc(c.ids.elementalDamage, pI->ids.elementalDamage);
        c.ids.earthDamage = maxc(c.ids.earthDamage, pI->ids.earthDamage);
        c.ids.thunderDamage = maxc(c.ids.thunderDamage, pI->ids.thunderDamage);
        c.ids.fireDamage = maxc(c.ids.fireDamage, pI->ids.fireDamage);
        c.ids.airDamage = maxc(c.ids.airDamage, pI->ids.airDamage);
        c.ids.waterDamage = maxc(c.ids.waterDamage, pI->ids.waterDamage);

        c.ids.elementalDefence = maxc(c.ids.elementalDefence, pI->ids.elementalDefence);
        c.ids.earthDefence = maxc(c.ids.earthDefence, pI->ids.earthDefence);
        c.ids.thunderDefence = maxc(c.ids.thunderDefence, pI->ids.thunderDefence);
        c.ids.fireDefence = maxc(c.ids.fireDefence, pI->ids.fireDefence);
        c.ids.airDefence = maxc(c.ids.airDefence, pI->ids.airDefence);
        c.ids.waterDefence = maxc(c.ids.waterDefence, pI->ids.waterDefence);

        c.ids.rawElementalDamage = maxc(c.ids.rawElementalDamage, pI->ids.rawElementalDamage);
        c.ids.rawEarthDamage = maxc(c.ids.rawEarthDamage, pI->ids.rawEarthDamage);
        c.ids.rawThunderDamage = maxc(c.ids.rawThunderDamage, pI->ids.rawThunderDamage);
        c.ids.rawFireDamage = maxc(c.ids.rawFireDamage, pI->ids.rawFireDamage);
        c.ids.rawAirDamage = maxc(c.ids.rawAirDamage, pI->ids.rawAirDamage);
        c.ids.rawWaterDamage = maxc(c.ids.rawWaterDamage, pI->ids.rawWaterDamage);

        c.ids.mainAttackDamage = maxc(c.ids.mainAttackDamage, pI->ids.mainAttackDamage);
        c.ids.earthMainAttackDamage = maxc(c.ids.earthMainAttackDamage, pI->ids.earthMainAttackDamage);
        c.ids.thunderMainAttackDamage = maxc(c.ids.thunderMainAttackDamage, pI->ids.thunderMainAttackDamage);
        c.ids.fireMainAttackDamage = maxc(c.ids.fireMainAttackDamage, pI->ids.fireMainAttackDamage);
        c.ids.airMainAttackDamage = maxc(c.ids.airMainAttackDamage, pI->ids.airMainAttackDamage);
        c.ids.waterMainAttackDamage = maxc(c.ids.waterMainAttackDamage, pI->ids.waterMainAttackDamage);

        c.ids.rawMainAttackDamage = maxc(c.ids.rawMainAttackDamage, pI->ids.rawMainAttackDamage);
        c.ids.rawElementalMainAttackDamage = maxc(c.ids.rawElementalMainAttackDamage, pI->ids.rawElementalMainAttackDamage);
        c.ids.rawEarthMainAttackDamage = maxc(c.ids.rawEarthMainAttackDamage, pI->ids.rawEarthMainAttackDamage);
        c.ids.rawThunderMainAttackDamage = maxc(c.ids.rawThunderMainAttackDamage, pI->ids.rawThunderMainAttackDamage);
        c.ids.rawFireMainAttackDamage = maxc(c.ids.rawFireMainAttackDamage, pI->ids.rawFireMainAttackDamage);
        c.ids.rawAirMainAttackDamage = maxc(c.ids.rawAirMainAttackDamage, pI->ids.rawAirMainAttackDamage);
        c.ids.rawWaterMainAttackDamage = maxc(c.ids.rawWaterMainAttackDamage, pI->ids.rawWaterMainAttackDamage);

        c.ids.spellDamage = maxc(c.ids.spellDamage, pI->ids.spellDamage);
        c.ids.elementalSpellDamage = maxc(c.ids.elementalSpellDamage, pI->ids.elementalSpellDamage);
        c.ids.earthSpellDamage = maxc(c.ids.earthSpellDamage, pI->ids.earthSpellDamage);
        c.ids.thunderSpellDamage = maxc(c.ids.thunderSpellDamage, pI->ids.thunderSpellDamage);
        c.ids.fireSpellDamage = maxc(c.ids.fireSpellDamage, pI->ids.fireSpellDamage);
        c.ids.airSpellDamage = maxc(c.ids.airSpellDamage, pI->ids.airSpellDamage);
        c.ids.waterSpellDamage = maxc(c.ids.waterSpellDamage, pI->ids.waterSpellDamage);

        c.ids.rawSpellDamage = maxc(c.ids.rawSpellDamage, pI->ids.rawSpellDamage);
        c.ids.rawNeutralSpellDamage = maxc(c.ids.rawNeutralSpellDamage, pI->ids.rawNeutralSpellDamage);
        c.ids.rawElementalSpellDamage = maxc(c.ids.rawElementalSpellDamage, pI->ids.rawElementalSpellDamage);
        c.ids.rawEarthSpellDamage = maxc(c.ids.rawEarthSpellDamage, pI->ids.rawEarthSpellDamage);
        c.ids.rawThunderSpellDamage = maxc(c.ids.rawThunderSpellDamage, pI->ids.rawThunderSpellDamage);
        c.ids.rawFireSpellDamage = maxc(c.ids.rawFireSpellDamage, pI->ids.rawFireSpellDamage);
        c.ids.rawAirSpellDamage = maxc(c.ids.rawAirSpellDamage, pI->ids.rawAirSpellDamage);
        c.ids.rawWaterSpellDamage = maxc(c.ids.rawWaterSpellDamage, pI->ids.rawWaterSpellDamage);

        c.ids.healing = maxc(c.ids.healing, pI->ids.healing);
        c.ids.rawHealth = maxc(c.ids.rawHealth, pI->ids.rawHealth);
        c.ids.healingEfficiency = maxc(c.ids.healingEfficiency, pI->ids.healingEfficiency);
        c.ids.healthRegen = maxc(c.ids.healthRegen, pI->ids.healthRegen);
        c.ids.healthRegenRaw = maxc(c.ids.healthRegenRaw, pI->ids.healthRegenRaw);
        c.ids.lifeSteal = maxc(c.ids.lifeSteal, pI->ids.lifeSteal);

        c.ids.manaRegen = maxc(c.ids.manaRegen, pI->ids.manaRegen);
        c.ids.manaSteal = maxc(c.ids.manaSteal, pI->ids.manaSteal);

        c.ids.sprint = maxc(c.ids.sprint, pI->ids.sprint);
        c.ids.sprintRegen = maxc(c.ids.sprintRegen, pI->ids.sprintRegen);
        c.ids.walkSpeed = maxc(c.ids.walkSpeed, pI->ids.walkSpeed);
        c.ids.jumpHeight = maxc(c.ids.jumpHeight, pI->ids.jumpHeight);

        c.ids.thorns = maxc(c.ids.thorns, pI->ids.thorns);
        c.ids.reflection = maxc(c.ids.reflection, pI->ids.reflection);
        c.ids.knockback = maxc(c.ids.knockback, pI->ids.knockback);
        c.ids.exploding = maxc(c.ids.exploding, pI->ids.exploding);
        c.ids.poison = maxc(c.ids.poison, pI->ids.poison);

        c.ids.stealing = maxc(c.ids.stealing, pI->ids.stealing);
        c.ids.xpBonus = maxc(c.ids.xpBonus, pI->ids.xpBonus);
        c.ids.lootBonus = maxc(c.ids.lootBonus, pI->ids.lootBonus);
        c.ids.soulPointRegen = maxc(c.ids.soulPointRegen, pI->ids.soulPointRegen); // May change in next update

        c.ids.SpellCost1st = maxc(c.ids.SpellCost1st, pI->ids.SpellCost1st);
        c.ids.SpellCost2nd = maxc(c.ids.SpellCost2nd, pI->ids.SpellCost2nd);
        c.ids.SpellCost3rd = maxc(c.ids.SpellCost3rd, pI->ids.SpellCost3rd);
        c.ids.SpellCost4th = maxc(c.ids.SpellCost4th, pI->ids.SpellCost4th);

        c.ids.rawSpellCost1st = maxc(c.ids.rawSpellCost1st, pI->ids.rawSpellCost1st);
        c.ids.rawSpellCost2nd = maxc(c.ids.rawSpellCost2nd, pI->ids.rawSpellCost2nd);
        c.ids.rawSpellCost3rd = maxc(c.ids.rawSpellCost3rd, pI->ids.rawSpellCost3rd);
        c.ids.rawSpellCost4th = maxc(c.ids.rawSpellCost4th, pI->ids.rawSpellCost4th);
        
        c.ids.slowEnemy = maxc(c.ids.slowEnemy, pI->ids.slowEnemy);
        c.ids.weakenEnemy = maxc(c.ids.weakenEnemy, pI->ids.weakenEnemy);
    }

    print_recommendation(c.type, "c.type");
    print_recommendation(c.tier, "c.tier");
    print_recommendation(c.powderSlots, "c.powderSlots");
    print_recommendation(c.attackSpeed, "c.attackSpeed");
    print_recommendation(c.reqs.class, "c.reqs.class");
    print_recommendation(c.reqs.level, "c.reqs.level");
    print_recommendation(c.reqs.strength, "c.reqs.strength");
    print_recommendation(c.reqs.dexterity, "c.reqs.dexterity");
    print_recommendation(c.reqs.defence, "c.reqs.defence");
    print_recommendation(c.reqs.agility, "c.reqs.agility");
    print_recommendation(c.reqs.intelligence, "c.reqs.intelligence");
    print_recommendation(c.base.averageDPS, "c.base.averageDPS");
    print_recommendation(c.base.damage, "c.base.damage");
    print_recommendation(c.base.earthDamage, "c.base.earthDamage");
    print_recommendation(c.base.thunderDamage, "c.base.thunderDamage");
    print_recommendation(c.base.fireDamage, "c.base.fireDamage");
    print_recommendation(c.base.airDamage, "c.base.airDamage");
    print_recommendation(c.base.waterDamage, "c.base.waterDamage");
    print_recommendation(c.base.health, "c.base.health");
    print_recommendation(c.base.earthDefence, "c.base.earthDefence");
    print_recommendation(c.base.thunderDefence, "c.base.thunderDefence");
    print_recommendation(c.base.fireDefence, "c.base.fireDefence");
    print_recommendation(c.base.airDefence, "c.base.airDefence");
    print_recommendation(c.base.waterDefence, "c.base.waterDefence");
    print_recommendation(c.ids.rawAttackSpeed, "c.ids.rawAttackSpeed");
    print_recommendation(c.ids.rawStrength, "c.ids.rawStrength");
    print_recommendation(c.ids.rawDexterity, "c.ids.rawDexterity");
    print_recommendation(c.ids.rawDefence, "c.ids.rawDefence");
    print_recommendation(c.ids.rawAgility, "c.ids.rawAgility");
    print_recommendation(c.ids.rawIntelligence, "c.ids.rawIntelligence");
    print_recommendation(c.ids.elementalDamage, "c.ids.elementalDamage");
    print_recommendation(c.ids.earthDamage, "c.ids.earthDamage");
    print_recommendation(c.ids.thunderDamage, "c.ids.thunderDamage");
    print_recommendation(c.ids.fireDamage, "c.ids.fireDamage");
    print_recommendation(c.ids.airDamage, "c.ids.airDamage");
    print_recommendation(c.ids.waterDamage, "c.ids.waterDamage");
    print_recommendation(c.ids.elementalDefence, "c.ids.elementalDefence");
    print_recommendation(c.ids.earthDefence, "c.ids.earthDefence");
    print_recommendation(c.ids.thunderDefence, "c.ids.thunderDefence");
    print_recommendation(c.ids.fireDefence, "c.ids.fireDefence");
    print_recommendation(c.ids.airDefence, "c.ids.airDefence");
    print_recommendation(c.ids.waterDefence, "c.ids.waterDefence");
    print_recommendation(c.ids.rawElementalDamage, "c.ids.rawElementalDamage");
    print_recommendation(c.ids.rawEarthDamage, "c.ids.rawEarthDamage");
    print_recommendation(c.ids.rawThunderDamage, "c.ids.rawThunderDamage");
    print_recommendation(c.ids.rawFireDamage, "c.ids.rawFireDamage");
    print_recommendation(c.ids.rawAirDamage, "c.ids.rawAirDamage");
    print_recommendation(c.ids.rawWaterDamage, "c.ids.rawWaterDamage");
    print_recommendation(c.ids.mainAttackDamage, "c.ids.mainAttackDamage");
    print_recommendation(c.ids.earthMainAttackDamage, "c.ids.earthMainAttackDamag");
    print_recommendation(c.ids.thunderMainAttackDamage, "c.ids.thunderMainAttackDam");
    print_recommendation(c.ids.fireMainAttackDamage, "c.ids.fireMainAttackDamage");
    print_recommendation(c.ids.airMainAttackDamage, "c.ids.airMainAttackDamage");
    print_recommendation(c.ids.waterMainAttackDamage, "c.ids.waterMainAttackDamag");
    print_recommendation(c.ids.rawMainAttackDamage, "c.ids.rawMainAttackDamage");
    print_recommendation(c.ids.rawElementalMainAttackDamage, "c.ids.rawElementalMainAtta");
    print_recommendation(c.ids.rawEarthMainAttackDamage, "c.ids.rawEarthMainAttackDa");
    print_recommendation(c.ids.rawThunderMainAttackDamage, "c.ids.rawThunderMainAttack");
    print_recommendation(c.ids.rawFireMainAttackDamage, "c.ids.rawFireMainAttackDam");
    print_recommendation(c.ids.rawAirMainAttackDamage, "c.ids.rawAirMainAttackDama");
    print_recommendation(c.ids.rawWaterMainAttackDamage, "c.ids.rawWaterMainAttackDa");
    print_recommendation(c.ids.spellDamage, "c.ids.spellDamage");
    print_recommendation(c.ids.elementalSpellDamage, "c.ids.elementalSpellDamage");
    print_recommendation(c.ids.earthSpellDamage, "c.ids.earthSpellDamage");
    print_recommendation(c.ids.thunderSpellDamage, "c.ids.thunderSpellDamage");
    print_recommendation(c.ids.fireSpellDamage, "c.ids.fireSpellDamage");
    print_recommendation(c.ids.airSpellDamage, "c.ids.airSpellDamage");
    print_recommendation(c.ids.waterSpellDamage, "c.ids.waterSpellDamage");
    print_recommendation(c.ids.rawSpellDamage, "c.ids.rawSpellDamage");
    print_recommendation(c.ids.rawNeutralSpellDamage, "c.ids.rawNeutralSpellDamag");
    print_recommendation(c.ids.rawElementalSpellDamage, "c.ids.rawElementalSpellDam");
    print_recommendation(c.ids.rawEarthSpellDamage, "c.ids.rawEarthSpellDamage");
    print_recommendation(c.ids.rawThunderSpellDamage, "c.ids.rawThunderSpellDamag");
    print_recommendation(c.ids.rawFireSpellDamage, "c.ids.rawFireSpellDamage");
    print_recommendation(c.ids.rawAirSpellDamage, "c.ids.rawAirSpellDamage");
    print_recommendation(c.ids.rawWaterSpellDamage, "c.ids.rawWaterSpellDamage");
    print_recommendation(c.ids.healing, "c.ids.healing");
    print_recommendation(c.ids.rawHealth, "c.ids.rawHealth");
    print_recommendation(c.ids.healingEfficiency, "c.ids.healingEfficiency");
    print_recommendation(c.ids.healthRegen, "c.ids.healthRegen");
    print_recommendation(c.ids.healthRegenRaw, "c.ids.healthRegenRaw");
    print_recommendation(c.ids.lifeSteal, "c.ids.lifeSteal");
    print_recommendation(c.ids.manaRegen, "c.ids.manaRegen");
    print_recommendation(c.ids.manaSteal, "c.ids.manaSteal");
    print_recommendation(c.ids.sprint, "c.ids.sprint");
    print_recommendation(c.ids.sprintRegen, "c.ids.sprintRegen");
    print_recommendation(c.ids.walkSpeed, "c.ids.walkSpeed");
    print_recommendation(c.ids.jumpHeight, "c.ids.jumpHeight");
    print_recommendation(c.ids.thorns, "c.ids.thorns");
    print_recommendation(c.ids.reflection, "c.ids.reflection");
    print_recommendation(c.ids.knockback, "c.ids.knockback");
    print_recommendation(c.ids.exploding, "c.ids.exploding");
    print_recommendation(c.ids.poison, "c.ids.poison");
    print_recommendation(c.ids.stealing, "c.ids.stealing");
    print_recommendation(c.ids.xpBonus, "c.ids.xpBonus");
    print_recommendation(c.ids.lootBonus, "c.ids.lootBonus");
    print_recommendation(c.ids.soulPointRegen, "c.ids.soulPointRegen");
    print_recommendation(c.ids.SpellCost1st, "c.ids.SpellCost1st");
    print_recommendation(c.ids.SpellCost2nd, "c.ids.SpellCost2nd");
    print_recommendation(c.ids.SpellCost3rd, "c.ids.SpellCost3rd");
    print_recommendation(c.ids.SpellCost4th, "c.ids.SpellCost4th");
    print_recommendation(c.ids.rawSpellCost1st, "c.ids.rawSpellCost1st");
    print_recommendation(c.ids.rawSpellCost2nd, "c.ids.rawSpellCost2nd");
    print_recommendation(c.ids.rawSpellCost3rd, "c.ids.rawSpellCost3rd");
    print_recommendation(c.ids.rawSpellCost4th, "c.ids.rawSpellCost4th");
    print_recommendation(c.ids.slowEnemy, "c.ids.slowEnemy");
    print_recommendation(c.ids.weakenEnemy, "c.ids.weakenEnemy");
}

#endif // INFO_UTILS_H