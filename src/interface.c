#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui/raygui.h>
#include <raygui/style_cyber.h>
#include "wynnitems.h"

static float values[sizeof(WynnItemIdArray)] = {0};

static void draw_slider(float x, float y, size_t id, const char* name)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle bounds = {1 + x * 151, 9 * y + 1, 150, 8};

    GuiSlider(bounds, NULL, NULL, &values[id], -1.f, 1.f);
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

        size_t baseCount = sizeof(wynnItemBaseNames) / sizeof(*wynnItemBaseNames);
        for (size_t i = 0; i < baseCount; ++i)
        {
            size_t down = id % 32;
            size_t right = id / 32;

            draw_slider((float)right, (float)down, id, wynnItemBaseNames[i]);
            id++;
        }

        size_t idCount = sizeof(wynnItemIdNames) / sizeof(*wynnItemIdNames);
        for (size_t i = 0; i < idCount; i++)
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