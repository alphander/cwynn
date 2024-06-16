#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui/raygui.h>

int iteminterface_run(void* pArgs)
{
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(400, 200, "raygui - controls test suite");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    float value = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        GuiSlider((Rectangle){100, 100, 100, 10}, "Something", NULL, &value, -1.0, 1.0);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}