#include "Graphics_HAL.h"

// Helper function
// Initializes the fonts for use on the LED screen
void InitFonts()
{
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

///
//  Game initialization
///

void initGraphics(Graphics_Context *g_sContext_p)
{
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);

    //drawGameDisplay(g_sContext_p);
}

void drawMaroonSquare(Graphics_Context* g_sContext_p, unsigned int xMin, unsigned int xMax, unsigned int yMin, unsigned int yMax)
{
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_MAROON);
    Graphics_Rectangle maroon;
        maroon.xMin = xMin;
        maroon.xMax = xMax;
        maroon.yMin = yMin;
        maroon.yMax = yMax;
    Graphics_fillRectangle(g_sContext_p, &maroon);
}

void drawGameDisplay(Graphics_Context *g_sContext_p)
{
    // Orange squares are easy - just fill the screen with an orange rectangle
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
    Graphics_Rectangle orangeSquares;
        orangeSquares.xMin = 0;
        orangeSquares.xMax = 127;
        orangeSquares.yMin = 0;
        orangeSquares.yMax = 127;
    Graphics_fillRectangle(g_sContext_p, &orangeSquares);

    drawMaroonSquare(g_sContext_p, a, b - 1, 0, r1-1);
    drawMaroonSquare(g_sContext_p, c, d-1, 0, r1-1);
    drawMaroonSquare(g_sContext_p, e, f-1, 0, r1-1);
    drawMaroonSquare(g_sContext_p, g, h-1, 0, r1-1);

    drawMaroonSquare(g_sContext_p, 0, a-1, r1, r2-1);
    drawMaroonSquare(g_sContext_p, b, c-1, r1, r2-1);
    drawMaroonSquare(g_sContext_p, d, e-1, r1, r2-1);
    drawMaroonSquare(g_sContext_p, f, g-1, r1, r2-1);

    drawMaroonSquare(g_sContext_p, a, b - 1, r2, r3-1);
    drawMaroonSquare(g_sContext_p, c, d-1, r2, r3-1);
    drawMaroonSquare(g_sContext_p, e, f-1, r2, r3-1);
    drawMaroonSquare(g_sContext_p, g, h-1, r2, r3-1);

    drawMaroonSquare(g_sContext_p, 0, a-1, r3, r4-1);
    drawMaroonSquare(g_sContext_p, b, c-1, r3, r4-1);
    drawMaroonSquare(g_sContext_p, d, e-1, r3, r4-1);
    drawMaroonSquare(g_sContext_p, f, g-1, r3, r4-1);

    drawMaroonSquare(g_sContext_p, a, b - 1, r4, r5-1);
    drawMaroonSquare(g_sContext_p, c, d-1, r4, r5-1);
    drawMaroonSquare(g_sContext_p, e, f-1, r4, r5-1);
    drawMaroonSquare(g_sContext_p, g, h-1, r4, r5-1);

    drawMaroonSquare(g_sContext_p, 0, a-1, r5, r6-1);
    drawMaroonSquare(g_sContext_p, b, c-1, r5, r6-1);
    drawMaroonSquare(g_sContext_p, d, e-1, r5, r6-1);
    drawMaroonSquare(g_sContext_p, f, g-1, r5, r6-1);

    drawMaroonSquare(g_sContext_p, a, b - 1, r6, r7-1);
    drawMaroonSquare(g_sContext_p, c, d-1, r6, r7-1);
    drawMaroonSquare(g_sContext_p, e, f-1, r6, r7-1);
    drawMaroonSquare(g_sContext_p, g, h-1, r6, r7-1);

    drawMaroonSquare(g_sContext_p, 0, a-1, r7, r8-1);
    drawMaroonSquare(g_sContext_p, b, c-1, r7, r8-1);
    drawMaroonSquare(g_sContext_p, d, e-1, r7, r8-1);
    drawMaroonSquare(g_sContext_p, f, g-1, r7, r8-1);
}

void drawOpeningSplash(Graphics_Context *g_sContext_p)
{
    Graphics_drawImage(g_sContext_p, &splash8BPP_COMP_RLE8, 0, 0);
}
