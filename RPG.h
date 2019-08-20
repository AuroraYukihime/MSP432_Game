// RPG Game Logic Struct

#ifndef RPG_Class
#define RPG_Class

#include "ADC_HAL.h"

#define HIGH_SCORES 3

typedef enum
{
    Menu, Dungeon, Battle
} gameState;

typedef struct
{
    // RNG
    counterType randomSeed;
    unsigned int heroLevel; // Richter's level
    unsigned int dracLevel; // Dracula's level

    // Game data
    gameState state;
    unsigned int bestScores[HIGH_SCORES]; // Keep top three of Richter's level every session
} RPG;

void initGame(RPG* game);

#endif //RPG Class
