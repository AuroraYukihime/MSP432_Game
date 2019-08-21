// RPG Game Logic Struct

#ifndef RPG_Class
#define RPG_Class

#include "ADC_HAL.h"
#include "Graphics_HAL.h"

#define HIGH_SCORES 3

typedef enum
{
    Menu, Dungeon, Battle
} gameState;

typedef struct
{
    Graphics_Context g_sContext;

    // RNG
    counterType randomSeed;
    unsigned int heroLevel; // Richter's level
    unsigned int dracLevel; // Dracula's level

    // Board data
    boardPosition richterPos; // Richter's position

    // Game data
    gameState state;
    unsigned int bestScores[HIGH_SCORES]; // Keep top three of Richter's level every session
} RPG;

void initGame(RPG* game);

#endif //RPG Class
