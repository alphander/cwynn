#include "wynnitems.h"
#include <math.h>

static inline float stat_distance(int32_t a, int32_t b)
{
    return ((float)b - a) * ((float)b - a);
}

float wynnitem_similarity(WynnItem* pItem, WynnItem* pTestItem)
{
    float v = 0;
    
    // Requirements
    v += stat_distance(pItem->attackSpeed,         pTestItem->attackSpeed);
    v += stat_distance(pItem->reqs.level,          pTestItem->reqs.level);
    v += stat_distance(pItem->reqs.strength,       pTestItem->reqs.strength);
    v += stat_distance(pItem->reqs.dexterity,      pTestItem->reqs.dexterity);
    v += stat_distance(pItem->reqs.defence,        pTestItem->reqs.defence);
    v += stat_distance(pItem->reqs.agility,        pTestItem->reqs.agility);
    v += stat_distance(pItem->reqs.intelligence,   pTestItem->reqs.intelligence);
    
    // Base
    v += stat_distance(pItem->base.averageDPS,     pItem->base.averageDPS);
    v += stat_distance(pItem->base.damage,         pItem->base.damage);
    v += stat_distance(pItem->base.earthDamage,    pItem->base.earthDamage);
    v += stat_distance(pItem->base.thunderDamage,  pItem->base.thunderDamage);
    v += stat_distance(pItem->base.fireDamage,     pItem->base.fireDamage);
    v += stat_distance(pItem->base.airDamage,      pItem->base.airDamage);
    v += stat_distance(pItem->base.waterDamage,    pItem->base.waterDamage);

    v += stat_distance(pItem->base.health,         pItem->base.health);
    v += stat_distance(pItem->base.earthDefence,   pItem->base.earthDefence);
    v += stat_distance(pItem->base.thunderDefence, pItem->base.thunderDefence);
    v += stat_distance(pItem->base.fireDefence,    pItem->base.fireDefence);
    v += stat_distance(pItem->base.airDefence,     pItem->base.airDefence);
    v += stat_distance(pItem->base.waterDefence,   pItem->base.waterDefence);

    // Identifications
    v += stat_distance(pItem->ids.rawAttackSpeed, pItem->ids.rawAttackSpeed);

    v += stat_distance(pItem->ids.rawStrength, pItem->ids.rawStrength);
    v += stat_distance(pItem->ids.rawDexterity, pItem->ids.rawDexterity);
    v += stat_distance(pItem->ids.rawDefence, pItem->ids.rawDefence);
    v += stat_distance(pItem->ids.rawAgility, pItem->ids.rawAgility);
    v += stat_distance(pItem->ids.rawIntelligence, pItem->ids.rawIntelligence);

    v += stat_distance(pItem->ids.elementalDamage, pItem->ids.elementalDamage);
    v += stat_distance(pItem->ids.earthDamage, pItem->ids.earthDamage);
    v += stat_distance(pItem->ids.thunderDamage, pItem->ids.thunderDamage);
    v += stat_distance(pItem->ids.fireDamage, pItem->ids.fireDamage);
    v += stat_distance(pItem->ids.airDamage, pItem->ids.airDamage);
    v += stat_distance(pItem->ids.waterDamage, pItem->ids.waterDamage);

    v += stat_distance(pItem->ids.elementalDefence, pItem->ids.elementalDefence);
    v += stat_distance(pItem->ids.earthDefence, pItem->ids.earthDefence);
    v += stat_distance(pItem->ids.airDefence, pItem->ids.airDefence);
    v += stat_distance(pItem->ids.fireDefence, pItem->ids.fireDefence);
    v += stat_distance(pItem->ids.thunderDefence, pItem->ids.thunderDefence);
    v += stat_distance(pItem->ids.waterDefence, pItem->ids.waterDefence);

    v += stat_distance(pItem->ids.rawElementalDamage, pItem->ids.rawElementalDamage);
    v += stat_distance(pItem->ids.rawEarthDamage, pItem->ids.rawEarthDamage);
    v += stat_distance(pItem->ids.rawThunderDamage, pItem->ids.rawThunderDamage);
    v += stat_distance(pItem->ids.rawFireDamage, pItem->ids.rawFireDamage);
    v += stat_distance(pItem->ids.rawAirDamage, pItem->ids.rawAirDamage);
    v += stat_distance(pItem->ids.rawWaterDamage, pItem->ids.rawWaterDamage);

    v += stat_distance(pItem->ids.mainAttackDamage, pItem->ids.mainAttackDamage);
    v += stat_distance(pItem->ids.earthMainAttackDamage, pItem->ids.earthMainAttackDamage);
    v += stat_distance(pItem->ids.thunderMainAttackDamage, pItem->ids.thunderMainAttackDamage);
    v += stat_distance(pItem->ids.fireMainAttackDamage, pItem->ids.fireMainAttackDamage);
    v += stat_distance(pItem->ids.airMainAttackDamage, pItem->ids.airMainAttackDamage);
    v += stat_distance(pItem->ids.waterMainAttackDamage, pItem->ids.waterMainAttackDamage);

    v += stat_distance(pItem->ids.rawMainAttackDamage, pItem->ids.rawMainAttackDamage);
    v += stat_distance(pItem->ids.rawElementalMainAttackDamage, pItem->ids.rawElementalMainAttackDamage);
    v += stat_distance(pItem->ids.rawEarthMainAttackDamage, pItem->ids.rawEarthMainAttackDamage);
    v += stat_distance(pItem->ids.rawThunderMainAttackDamage, pItem->ids.rawThunderMainAttackDamage);
    v += stat_distance(pItem->ids.rawFireMainAttackDamage, pItem->ids.rawFireMainAttackDamage);
    v += stat_distance(pItem->ids.rawAirMainAttackDamage, pItem->ids.rawAirMainAttackDamage);
    v += stat_distance(pItem->ids.rawWaterMainAttackDamage, pItem->ids.rawWaterMainAttackDamage);

    v += stat_distance(pItem->ids.spellDamage, pItem->ids.spellDamage);
    v += stat_distance(pItem->ids.elementalSpellDamage, pItem->ids.elementalSpellDamage);
    v += stat_distance(pItem->ids.fireSpellDamage, pItem->ids.fireSpellDamage);
    v += stat_distance(pItem->ids.waterSpellDamage, pItem->ids.waterSpellDamage);
    v += stat_distance(pItem->ids.earthSpellDamage, pItem->ids.earthSpellDamage);
    v += stat_distance(pItem->ids.thunderSpellDamage, pItem->ids.thunderSpellDamage);
    v += stat_distance(pItem->ids.airSpellDamage, pItem->ids.airSpellDamage);

    v += stat_distance(pItem->ids.rawSpellDamage,   pItem->ids.rawSpellDamage);
    v += stat_distance(pItem->ids.rawNeutralSpellDamage, pItem->ids.rawNeutralSpellDamage);
    v += stat_distance(pItem->ids.rawElementalSpellDamage, pItem->ids.rawElementalSpellDamage);
    v += stat_distance(pItem->ids.rawEarthSpellDamage, pItem->ids.rawEarthSpellDamage);
    v += stat_distance(pItem->ids.rawThunderSpellDamage, pItem->ids.rawThunderSpellDamage);
    v += stat_distance(pItem->ids.rawFireSpellDamage, pItem->ids.rawFireSpellDamage);
    v += stat_distance(pItem->ids.rawAirSpellDamage, pItem->ids.rawAirSpellDamage);
    v += stat_distance(pItem->ids.rawWaterSpellDamage, pItem->ids.rawWaterSpellDamage);

    v += stat_distance(pItem->ids.healing, pItem->ids.healing);
    v += stat_distance(pItem->ids.rawHealth, pItem->ids.rawHealth);
    v += stat_distance(pItem->ids.healingEfficiency, pItem->ids.healingEfficiency);
    v += stat_distance(pItem->ids.healthRegen, pItem->ids.healthRegen);
    v += stat_distance(pItem->ids.healthRegenRaw, pItem->ids.healthRegenRaw);
    v += stat_distance(pItem->ids.lifeSteal, pItem->ids.lifeSteal);

    v += stat_distance(pItem->ids.manaRegen, pItem->ids.manaRegen);
    v += stat_distance(pItem->ids.manaSteal, pItem->ids.manaSteal);

    v += stat_distance(pItem->ids.sprint, pItem->ids.sprint);
    v += stat_distance(pItem->ids.sprintRegen, pItem->ids.sprintRegen);
    v += stat_distance(pItem->ids.walkSpeed, pItem->ids.walkSpeed);
    v += stat_distance(pItem->ids.jumpHeight, pItem->ids.jumpHeight);

    v += stat_distance(pItem->ids.thorns, pItem->ids.thorns);
    v += stat_distance(pItem->ids.reflection, pItem->ids.reflection);
    v += stat_distance(pItem->ids.knockback, pItem->ids.knockback);
    v += stat_distance(pItem->ids.exploding, pItem->ids.exploding);
    v += stat_distance(pItem->ids.poison, pItem->ids.poison);

    v += stat_distance(pItem->ids.stealing, pItem->ids.stealing);
    v += stat_distance(pItem->ids.xpBonus, pItem->ids.xpBonus);
    v += stat_distance(pItem->ids.lootBonus, pItem->ids.lootBonus);
    v += stat_distance(pItem->ids.soulPointRegen, pItem->ids.soulPointRegen);

    v += stat_distance(pItem->ids.SpellCost1st, pItem->ids.SpellCost1st);
    v += stat_distance(pItem->ids.SpellCost2nd, pItem->ids.SpellCost2nd);
    v += stat_distance(pItem->ids.SpellCost3rd, pItem->ids.SpellCost3rd);
    v += stat_distance(pItem->ids.SpellCost4th, pItem->ids.SpellCost4th);

    v += stat_distance(pItem->ids.rawSpellCost1st, pItem->ids.rawSpellCost1st);
    v += stat_distance(pItem->ids.rawSpellCost2nd, pItem->ids.rawSpellCost2nd);
    v += stat_distance(pItem->ids.rawSpellCost3rd, pItem->ids.rawSpellCost3rd);
    v += stat_distance(pItem->ids.rawSpellCost4th, pItem->ids.rawSpellCost4th);

    v += stat_distance(pItem->ids.slowEnemy, pItem->ids.slowEnemy);
    v += stat_distance(pItem->ids.weakenEnemy, pItem->ids.weakenEnemy);

    return sqrtf(v);
}