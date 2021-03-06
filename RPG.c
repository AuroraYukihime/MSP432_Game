#include "RPG.h"

void initGame(RPG* game)
{
    // Initialize game variables
    game->state = Menu;
    game->heroLevel = 1;
    game->dracLevel = 1;

    // Set Richter's default position
    game->richterPos.x = 12;
    game->richterPos.y = 12;
    game->richterPos.maroon = false;

    // Set high scores to 0
    game->bestScores[0] = 0;
    game->bestScores[1] = 0;
    game->bestScores[2] = 0;

    // Initialize random generator
    game->randomSeed.count = 0;
    game->randomSeed.ones = 0;
    game->randomSeed.zeroes = 0;
}
