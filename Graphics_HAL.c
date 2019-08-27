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
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);

    //drawGameDisplay(g_sContext_p);
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
    switch (cursor)
    {
    case 0:
        // Main Menu - Play option
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 55);
        break;
    case 1:
        // Main Menu - High Scores option
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 75);
        break;
    case 2:
        // Main Menu - Tutorial option
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 10, 95);
        break;
    }
}

void drawHighScores(Graphics_Context *g_sContext_p, const int score1,
                    const int score2, const int score3)
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
//  Dungeon Screen
///

void drawMaroonSquare(Graphics_Context* g_sContext_p, unsigned int xMin,
                      unsigned int xMax, unsigned int yMin, unsigned int yMax)
{
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_MAROON);
    Graphics_Rectangle maroon;
    maroon.xMin = xMin;
    maroon.xMax = xMax;
    maroon.yMin = yMin;
    maroon.yMax = yMax;
    Graphics_fillRectangle(g_sContext_p, &maroon);
}

void drawDungeonDisplay(Graphics_Context *g_sContext_p,
                        boardPosition richterPos)
{
    // Clear the screen
    clearScreen(g_sContext_p);

    // Orange squares are easy - just fill the screen with an orange rectangle
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
    Graphics_Rectangle orangeSquares;
    orangeSquares.xMin = 0;
    orangeSquares.xMax = 124;
    orangeSquares.yMin = 0;
    orangeSquares.yMax = 124;
    Graphics_fillRectangle(g_sContext_p, &orangeSquares);

    // Draw row 1
    drawMaroonSquare(g_sContext_p, a, b - 1, 0, r1 - 1);
    drawMaroonSquare(g_sContext_p, c, d - 1, 0, r1 - 1);

    // Draw row 2
    drawMaroonSquare(g_sContext_p, 0, a - 1, r1, r2 - 1);
    drawMaroonSquare(g_sContext_p, b, c - 1, r1, r2 - 1);
    drawMaroonSquare(g_sContext_p, d, e - 1, r1, r2 - 1);

    // Draw row 3
    drawMaroonSquare(g_sContext_p, a, b - 1, r2, r3 - 1);
    drawMaroonSquare(g_sContext_p, c, d - 1, r2, r3 - 1);

    // Draw row 4
    drawMaroonSquare(g_sContext_p, 0, a - 1, r3, r4 - 1);
    drawMaroonSquare(g_sContext_p, b, c - 1, r3, r4 - 1);
    drawMaroonSquare(g_sContext_p, d, e - 1, r3, r4 - 1);

    // Draw row 5
    drawMaroonSquare(g_sContext_p, a, b - 1, r4, r5 - 1);
    drawMaroonSquare(g_sContext_p, c, d - 1, r4, r5 - 1);

    // Draw Richter position
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(g_sContext_p, richterPos.x, richterPos.y, 5);
}

// Helper function - Clears player marker from Dungeon grid
void clear_player(Graphics_Context* g_sContext_p, int x, int y, bool current)
{
    if (current == true)
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_MAROON);
        Graphics_fillCircle(g_sContext_p, x, y, 5);
    }
    else
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
        Graphics_fillCircle(g_sContext_p, x, y, 5);
    }
}

void moveRichterUp(Graphics_Context *g_sContext_p, boardPosition *richterPos)
{
    // Remove the current player cursor
    clear_player(g_sContext_p, richterPos->x, richterPos->y,
                 richterPos->maroon);

    // Redraw the player cursor in updated position
    richterPos->y -= 25;
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(g_sContext_p, richterPos->x, richterPos->y, 5);

    // Update maroon boolean
    if (richterPos->maroon)
        richterPos->maroon = false;
    else
        richterPos->maroon = true;
}

void moveRichterDown(Graphics_Context *g_sContext_p, boardPosition *richterPos)
{
    // Remove the current player cursor
    clear_player(g_sContext_p, richterPos->x, richterPos->y,
                 richterPos->maroon);

    // Redraw the player cursor in updated position
    richterPos->y += 25;
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(g_sContext_p, richterPos->x, richterPos->y, 5);

    // Update maroon boolean
    if (richterPos->maroon)
        richterPos->maroon = false;
    else
        richterPos->maroon = true;
}

void moveRichterLeft(Graphics_Context *g_sContext_p, boardPosition *richterPos)
{
    // Remove the current player cursor
    clear_player(g_sContext_p, richterPos->x, richterPos->y,
                 richterPos->maroon);

    // Redraw the player cursor in updated position
    richterPos->x -= 25;
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(g_sContext_p, richterPos->x, richterPos->y, 5);

    // Update maroon boolean
    if (richterPos->maroon)
        richterPos->maroon = false;
    else
        richterPos->maroon = true;
}

void moveRichterRight(Graphics_Context *g_sContext_p, boardPosition *richterPos)
{
    // Remove the current player cursor
    clear_player(g_sContext_p, richterPos->x, richterPos->y,
                 richterPos->maroon);

    // Redraw the player cursor in updated position
    richterPos->x += 25;
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(g_sContext_p, richterPos->x, richterPos->y, 5);

    // Update maroon boolean
    if (richterPos->maroon)
        richterPos->maroon = false;
    else
        richterPos->maroon = true;
}

///
//  Battle Screen
///

void drawBattleDisplay(Graphics_Context *g_sContext_p, bool water, bool fire,int health)
{
    // Clear the screen
    clearScreen(g_sContext_p);

    // Draw menu window
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_Rectangle menuWindow;
    menuWindow.xMin = 27;
    menuWindow.xMax = 127;
    menuWindow.yMin = 84;
    menuWindow.yMax = 127;
    Graphics_fillRectangle(g_sContext_p, &menuWindow);

    // Draw menu border
    Graphics_setForegroundColor(g_sContext_p, COMMON_FOREGROUND);
    Graphics_Rectangle menuBorder;
    menuBorder.xMin = 27;
    menuBorder.xMax = 127;
    menuBorder.yMin = 84;
    menuBorder.yMax = 127;
    Graphics_drawRectangle(g_sContext_p, &menuBorder);

    // Draw menu options
    // TODO: Fire Cross and Holy Water will only appear if picked up in Dungeon screen
    int8_t fightOption[30] = "V. Killer ";
    int8_t fireMagic[30] = "F. Cross ";
    int8_t waterMagic[30] = "H. Water ";
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_drawString(g_sContext_p, fightOption, -1, 30, 85, true);
    Graphics_drawString(g_sContext_p, fireMagic, -1, 30, 100, true);
    Graphics_drawString(g_sContext_p, waterMagic, -1, 30, 115, true);

    // Draw default cursor
    drawBattleCursor(g_sContext_p, 0, water, fire);

    // Draw Dracula!
    Graphics_drawImage(g_sContext_p, &draculaDefault8BPP_UNCOMP, 25, 20);

    // Draw information box
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_Rectangle infoWindow;
    infoWindow.xMin = 0;
    infoWindow.xMax = 127;
    infoWindow.yMin = 0;
    infoWindow.yMax = 15;
    Graphics_fillRectangle(g_sContext_p, &infoWindow);

    // Draw information window border
    Graphics_setForegroundColor(g_sContext_p, COMMON_FOREGROUND);
    Graphics_Rectangle infoBorder;
    infoBorder.xMin = 0;
    infoBorder.xMax = 127;
    infoBorder.yMin = 0;
    infoBorder.yMax = 15;
    Graphics_drawRectangle(g_sContext_p, &infoBorder);
    // Draw information box text
    int8_t infoText[30] = "Vampire Killer ";
    Graphics_drawString(g_sContext_p, infoText, -1, 1, 1, true);

    // Health display

    int8_t dracName[30] = "Dracula ";
    int8_t dracHealth[30] = "100/100 ";
    make_3digit_NumString(health,(char*)dracHealth);
    int8_t richName[30] = "Richter ";
    dracHealth[3]='/';
    dracHealth[4]=dracHealth[0];
    dracHealth[5]=dracHealth[1];
    dracHealth[6]=dracHealth[2];
    int8_t richHealth[30] = "100/100 ";
    Graphics_setBackgroundColor(g_sContext_p, COMMON_BACKGROUND);
    Graphics_drawString(g_sContext_p, dracName, -1, 65, 20, true);
    Graphics_drawString(g_sContext_p, dracHealth, -1, 65, 30, true);
    Graphics_drawString(g_sContext_p, richName, -1, 65, 60, true);
    Graphics_drawString(g_sContext_p, richHealth, -1, 65, 70, true);
}

void drawBattleCursor(Graphics_Context *g_sContext_p, const int cursor,
bool water,
                      bool fire)
{
    // Erase previous cursors
    Graphics_Rectangle eraseCursor;
    eraseCursor.xMin = 5;
    eraseCursor.xMax = 25;
    eraseCursor.yMin = 85;
    eraseCursor.yMax = 126;

    Graphics_setForegroundColor(g_sContext_p, COMMON_BACKGROUND);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_fillRectangle(g_sContext_p, &eraseCursor);

    // Draw new cursor
    switch (cursor)
    {
    case 0:
        Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 5, 85);

        // Draw information box text
        clearInfoBox(g_sContext_p);
        int8_t infoText0[30] = "Vampire Killer ";
        Graphics_drawString(g_sContext_p, infoText0, -1, 1, 1, true);
        break;
    case 1:
        if (water)
        {
            Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 5, 100);

            // Draw information box text
            clearInfoBox(g_sContext_p);
            int8_t infoText1[30] = "Fire Cross ";
            Graphics_drawString(g_sContext_p, infoText1, -1, 1, 1, true);
        }
        else
        {
            Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 5, 100);

            // Draw information box text
            clearInfoBox(g_sContext_p);
            int8_t infoText1[30] = " No Fire Cross ";
            Graphics_drawString(g_sContext_p, infoText1, -1, 1, 1, true);
        }
        break;
    case 2:
        if (fire)
        {
            Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 5, 115);

            // Draw information box text
            clearInfoBox(g_sContext_p);
            int8_t infoText2[30] = "Holy Water ";
            Graphics_drawString(g_sContext_p, infoText2, -1, 1, 1, true);
        }
        else
        {
            Graphics_drawImage(g_sContext_p, &sord4BPP_UNCOMP, 5, 115);

            // Draw information box text
            clearInfoBox(g_sContext_p);
            int8_t infoText2[30] = "No Holy Water ";
            Graphics_drawString(g_sContext_p, infoText2, -1, 1, 1, true);
        }
        break;
    }
}

///
//  Clearing Operations
///

void clearScreen(Graphics_Context *g_sContext_p)
{
    Graphics_setBackgroundColor(g_sContext_p, COMMON_BACKGROUND);
    Graphics_clearDisplay(g_sContext_p);
}

void clearInfoBox(Graphics_Context *g_sContext_p)
{
    // Draw information box
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_Rectangle infoWindow;
    infoWindow.xMin = 0;
    infoWindow.xMax = 127;
    infoWindow.yMin = 0;
    infoWindow.yMax = 15;
    Graphics_fillRectangle(g_sContext_p, &infoWindow);

    // Draw information window border
    Graphics_setForegroundColor(g_sContext_p, COMMON_FOREGROUND);
    Graphics_Rectangle infoBorder;
    infoBorder.xMin = 0;
    infoBorder.xMax = 127;
    infoBorder.yMin = 0;
    infoBorder.yMax = 15;
    Graphics_drawRectangle(g_sContext_p, &infoBorder);
}
void make_3digit_NumString(unsigned int num, char *string)//these were used for testing
{
    string[0] = (num / 100) + '0';
    string[1] = ((num % 100) / 10) + '0';
    string[2] = ((num % 10) / 1) + '0';
    string[3] = 0;
}
