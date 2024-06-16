#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui/raygui.h>
#include <raygui/style_cyber.h>


static char* sliderNames[] = {
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


        for (size_t i = 0; i < count; i++)
        {
            char* name = sliderNames[i];
            GuiSlider((Rectangle){1, 10 * i + 1, 100, 9}, NULL, name, &values[i], -1.f, 1.f);
        }

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}