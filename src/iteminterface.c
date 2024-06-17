#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui/raygui.h>
#include <raygui/style_cyber.h>


static char* sliderNames[] = {
    // Base
    // Average DPS
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

static float values[sizeof(sliderNames) / sizeof(char*)] = {0};
static const size_t count = sizeof(sliderNames) / sizeof(char*);


int iteminterface_run(void* pArgs)
{
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(120);
    
    InitWindow(1400, 800, "");
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    GuiLoadStyleCyber();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);

    while (!WindowShouldClose())
    {
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        float guiScale = GetScreenHeight() / 300.f;
        BeginDrawing();
        SetMouseScale(1.f / guiScale, 1.f / guiScale);
        BeginMode2D((Camera2D){{0}, {0}, 0, guiScale});

        Vector2 mousePos = GetMousePosition();

        for (size_t i = 0, right = 0, down = 0; i < count; i++)
        {

            Rectangle bounds = {1 + right * 151, 9 * down + 1, 150, 8};
            char* name = sliderNames[i];

            GuiSlider(bounds, NULL, NULL, &values[i], -1.f, 1.f);
            if (!CheckCollisionPointRec(mousePos, bounds))
                GuiDrawText(name, bounds, 0, WHITE);

            down++;
            if ((i + 1) % 32 == 0) 
            {
                right++;
                down = 0;
            }
        }

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}