#ifndef GRAPHICS_HAL_H_
#define GRAPHICS_HAL_H_

// System includes
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Graphics intentionally has no module includes

#define COMMON_FOREGROUND GRAPHICS_COLOR_WHITE
#define COMMON_BACKGROUND GRAPHICS_COLOR_BLACK

// Bottom edge of rows
#define r1 25
#define r2 50
#define r3 75
#define r4 100
#define r5 125

// Right edges of squares
#define a 25
#define b 50
#define c 75
#define d 100
#define e 125

typedef struct
{
    int x;
    int y;
    bool maroon;
}boardPosition;

extern Graphics_Image title8BPP_UNCOMP;
extern Graphics_Image title_logo4BPP_UNCOMP;
extern Graphics_Image sord4BPP_UNCOMP;
extern Graphics_Image draculaDefault8BPP_UNCOMP;

///
//  Game initialization
///

// Prepares the graphics context for the game display
void initGraphics(Graphics_Context *g_sContext_p);

///
//  Splash Screens
///

// Draws the opening splash screen at program start
void drawOpeningSplash(Graphics_Context *g_sContext_p);

///
//  Main Menu
///

void drawMainMenu(Graphics_Context *g_sContext_p);
void drawMenuCursor(Graphics_Context *g_sContext_p, const int cursor);
void drawHighScores(Graphics_Context *g_sContext_p, const int score1, const int score2, const int score3);
void drawHelpScreen(Graphics_Context *g_sContext_p);

///
//  Dungeon Screen
///

// Draws the initial state of the game
void drawDungeonDisplay(Graphics_Context *g_sContext_p, boardPosition richterPos);

// These move the player cursor around the Dungeon map
// Returns whether Richter is on a maroon square (true) or orange square (false)
void moveRichterUp(Graphics_Context *g_sContext_p, boardPosition *richterPos);
void moveRichterDown(Graphics_Context *g_sContext_p, boardPosition *richterPos);
void moveRichterLeft(Graphics_Context *g_sContext_p, boardPosition *richterPos);
void moveRichterRight(Graphics_Context *g_sContext_p, boardPosition *richterPos);

///
//  Battle Screen
///

// Draws the battle display
void drawBattleDisplay(Graphics_Context *g_sContext_p);
void drawBattleCursor(Graphics_Context *g_sContext_p, const int cursor);

///
//  Clearing Operations
///

void clearScreen(Graphics_Context *g_sContext_p);

void clearInfoBox(Graphics_Context *g_sContext_p);

#endif
