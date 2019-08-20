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

///
//  Splash Screens
///

void drawOpeningSplash(Graphics_Context *g_sContext_p)
{
    Graphics_drawImage(g_sContext_p, &title8BPP_UNCOMP, 0, 0);
}

///
//  Main Menu
///

void drawMainMenu(Graphics_Context *g_sContext_p)
{
    clearScreen(g_sContext_p);

    Graphics_drawImage(g_sContext_p, &title_logo4BPP_UNCOMP, 4, 0);
    int8_t playOption[30] = "Begin Quest! ";
    int8_t infoOption[30] = "How to Play ";
    int8_t highOption[30] = "Top Levels ";
    int8_t copyright[30] = "Just Monika 2019";

    Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 55);

    Graphics_drawString(g_sContext_p, playOption, -1, 30, 55, true);
    Graphics_drawString(g_sContext_p, highOption, -1, 30, 75, true);
    Graphics_drawString(g_sContext_p, infoOption, -1, 30, 95, true);
    Graphics_drawString(g_sContext_p, copyright, -1, 0, 115, true);
}

void drawMenuCursor(Graphics_Context *g_sContext_p, const int cursor)
{
    // Erase previous cursors
    Graphics_Rectangle eraseCursor;
    eraseCursor.xMin = 10;
    eraseCursor.xMax = 30;
    eraseCursor.yMin = 55;
    eraseCursor.yMax = 105;

    Graphics_setForegroundColor(g_sContext_p, COMMON_BACKGROUND);
    Graphics_fillRectangle(g_sContext_p, &eraseCursor);
    Graphics_setForegroundColor(g_sContext_p, COMMON_FOREGROUND);

    // Draw new cursor
    switch(cursor)
    {
    case 0:
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 55);
        break;
    case 1:
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 75);
        break;
    case 2:
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 95);
        break;
    }
}
void drawHighScores(Graphics_Context *g_sContext_p, const int score1, const int score2, const int score3)
{
    clearScreen(g_sContext_p);

    int8_t first[3];
    int8_t second[3];
    int8_t third[3];

    int8_t info[30] = "Hall of Heroes ";

    first[1] = (score1 % 10) + '0';
    first[0] = (score1 / 10) + '0';

    second[1] = (score2 % 10) + '0';
    second[0] = (score2 / 10) + '0';

    third[1] = (score3 % 10) + '0';
    third[0] = (score3 / 10) + '0';

    Graphics_drawString(g_sContext_p, info, -1, 5, 5, true);
    Graphics_drawString(g_sContext_p, first, -1, 30, 50, true);
    Graphics_drawString(g_sContext_p, second, -1, 30, 70, true);
    Graphics_drawString(g_sContext_p, third, -1, 30, 90, true);
}
void drawHelpScreen(Graphics_Context *g_sContext_p)
{
    clearScreen(g_sContext_p);

    int8_t line0[30] = "How to Play:";
    int8_t line1[30] = "Help text in";
    int8_t line2[30] = "UART console";
    int8_t line3[30] = "";
    int8_t line4[30] = "Press key 1:";
    int8_t line5[30] = "Dungeon help";
    int8_t line6[30] = "Press key 2:";
    int8_t line7[30] = "Battle help";

    Graphics_drawString(g_sContext_p, line0, -1, 20, 0, true);
    Graphics_drawString(g_sContext_p, line1, -1, 0, 15, true);
    Graphics_drawString(g_sContext_p, line2, -1, 0, 25, true);
    Graphics_drawString(g_sContext_p, line3, -1, 0, 35, true);
    Graphics_drawString(g_sContext_p, line4, -1, 0, 45, true);
    Graphics_drawString(g_sContext_p, line5, -1, 0, 55, true);
    Graphics_drawString(g_sContext_p, line6, -1, 0, 75, true);
    Graphics_drawString(g_sContext_p, line7, -1, 0, 85, true);
}

///
//  Clearing Operations
///

void clearScreen(Graphics_Context *g_sContext_p)
{
    Graphics_clearDisplay(g_sContext_p);
}
