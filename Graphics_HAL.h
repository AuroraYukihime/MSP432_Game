#ifndef GRAPHICS_HAL_H_
#define GRAPHICS_HAL_H_

// System includes
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Bottom edge of rows
#define r1 16
#define r2 32
#define r3 48
#define r4 64
#define r5 80
#define r6 96
#define r7 112
#define r8 128

// Right edges of squares
#define a 16
#define b 32
#define c 48
#define d 64
#define e 80
#define f 96
#define g 112
#define h 128

extern Graphics_Image splash8BPP_COMP_RLE8;

///
//  Game initialization
///

// Prepares the graphics context for the game display
void initGraphics(Graphics_Context *g_sContext_p);

// Draws the initial state of the game
void drawGameDisplay(Graphics_Context *g_sContext_p);

///
//  Splash Screens
///

// Draws the opening splash screen at program start
void drawOpeningSplash(Graphics_Context *g_sContext_p);

#endif
