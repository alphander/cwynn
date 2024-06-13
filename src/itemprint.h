#include <LTK/jsonparser.h>
#include "wynnitems.h"

SET_GENERIC_EX(JsonKey, KeySet, keyset);

static const char* tier_to_name(WynnItemTier itemTier)
{
    switch(itemTier)
    {
        case WYNNITEM_TIER_COMMON: return "common";
        case WYNNITEM_TIER_UNIQUE: return "unique";
        case WYNNITEM_TIER_RARE: return "rare";
        case WYNNITEM_TIER_LEGENDARY: return "legendary";
        case WYNNITEM_TIER_FABLED: return "fabled";
        case WYNNITEM_TIER_MYTHIC: return "mythic";
        case WYNNITEM_TIER_SET: return "set";
    }
}

static const char* type_to_name(WynnItemType itemType)
{
    switch(itemType)
    {
        case WYNNITEM_TYPE_HELMET: return  "helmet";
        case WYNNITEM_TYPE_CHESTPLATE: return "chestplate";
        case WYNNITEM_TYPE_LEGGINGS: return "leggings";
        case WYNNITEM_TYPE_BOOTS: return "boots";
        case WYNNITEM_TYPE_RING: return "ring";
        case WYNNITEM_TYPE_BRACELET: return "bracelet";
        case WYNNITEM_TYPE_NECKLACE: return "necklate";
        case WYNNITEM_TYPE_WEAPON: return "weapon";
    }
}

static const char* speed_to_name(int32_t itemSpeed)
{
    switch (itemSpeed)
    {
        case WYNNITEM_ATTACK_SPEED_SUPER_SLOW: return "super_slow";
        case WYNNITEM_ATTACK_SPEED_VERY_SLOW: return "very_slow";
        case WYNNITEM_ATTACK_SPEED_SLOW: return "slow";
        case WYNNITEM_ATTACK_SPEED_NORMAL: return "normal";
        case WYNNITEM_ATTACK_SPEED_FAST: return "fast";
        case WYNNITEM_ATTACK_SPEED_VERY_FAST: return "very_fast";
        case WYNNITEM_ATTACK_SPEED_SUPER_FAST: return "super_fast";
        default: return "(error)";
    }
}

static const char* class_to_name(WynnItemClass itemClass)
{
    switch (itemClass)
    {
        case WYNNITEM_CLASS_MAGE: return "mage";
        case WYNNITEM_CLASS_WARRIOR: return "warrior";
        case WYNNITEM_CLASS_ARCHER: return "archer";
        case WYNNITEM_CLASS_ASSASSIN: return "assassin";
        case WYNNITEM_CLASS_SHAMAN: return "shaman";
    }
}

static void name_entries(JsonValue* pRoot)
{
    JsonValue* pJsonItem = NULL;
    JsonKey itemName = {0};

    KeySet reqsSet = keyset_create();
    KeySet baseSet = keyset_create();
    KeySet idSet = keyset_create();
    while ((pJsonItem = json_object_next(pRoot, &itemName)))
    {
        JsonValue* pTypeStr = NULL;
        if (!((pTypeStr = json_object_get(pJsonItem, (JsonKey){"type"})) ||
            (pTypeStr = json_object_get(pJsonItem, (JsonKey){"accessoryType"}))))
        continue;

        JsonValue* pReqsSection = json_object_get(pJsonItem, (JsonKey){"requirements"});
        if (pReqsSection != NULL)
        {
            JsonKey key = {0};
            while (json_object_next(pReqsSection, &key))
            {
                keyset_put(&reqsSet, key);
            }
        }

        JsonValue* pBaseSection = json_object_get(pJsonItem, (JsonKey){"base"});
        if (pBaseSection != NULL)
        {
            JsonKey key = {0};
            while (json_object_next(pBaseSection, &key))
            {
                keyset_put(&baseSet, key);
            }
        }

        JsonValue* pIdsSection = json_object_get(pJsonItem, (JsonKey){"identifications"});
        if (pIdsSection != NULL)
        {
            JsonKey key = {0};
            while (json_object_next(pIdsSection, &key))
            {
                keyset_put(&idSet, key);
            }
        }
    }

    JsonKey keyOut = {0};
    while (keyset_iter_next(&reqsSet, &keyOut))
    {
        printf("requirements: %s\n", keyOut.str);
    }
    while (keyset_iter_next(&baseSet, &keyOut))
    {
        printf("base: %s\n", keyOut.str);
    }
    while (keyset_iter_next(&idSet, &keyOut))
    {
        printf("identifications: %s\n", keyOut.str);
    }
    keyset_destroy(&reqsSet);
    keyset_destroy(&baseSet);
    keyset_destroy(&idSet);
}