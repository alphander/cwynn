#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui/raygui.h>
#include <raygui/style_cyber.h>
#include "wynnitems.h"

static void draw_slider(float x, float y, size_t id, const char* name)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle bounds = {1 + x * 151, 9 * y + 1, 150, 8};

    float value = wynnitem_get_value(id);
    if (GuiSlider(bounds, NULL, NULL, &value, -.5f, .5f))
    {
        wynnitem_set_value(id, value);
        // SUPER TEMP
        WynnBuild bestBuild = wynnitems_calculate_build(10000);

        for (size_t i = 0; i < 9; ++i)
        {
            printf("%s\n", bestBuild.pItems[i]->pName->str);
        }
        printf("\n");
    }
    if (!CheckCollisionPointRec(mousePos, bounds))
        GuiDrawText(name, bounds, 0, WHITE);
}

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

        float guiScale = GetScreenWidth() / 580.f;
        BeginDrawing();
        SetMouseScale(1.f / guiScale, 1.f / guiScale);
        BeginMode2D((Camera2D){{0}, {0}, 0, guiScale});

        size_t id = 0;
        for (size_t i = 0; i < lengthof(wynnItemBaseNames); ++i)
        {
            size_t down = id % 32;
            size_t right = id / 32;
            draw_slider((float)right, (float)down, id, wynnItemBaseNames[i]);
            id++;
        }

        for (size_t i = 0; i < lengthof(wynnItemIdNames); i++)
        {
            size_t down = id % 32;
            size_t right = id / 32;
            draw_slider((float)right, (float)down, id, wynnItemIdNames[i]);
            id++;
        }
        
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}