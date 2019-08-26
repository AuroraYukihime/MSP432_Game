// System Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Module Includes
#include "ADC_HAL.h"
#include "ButtonLED_HAL.h"
#include "Graphics_HAL.h"
#include "RPG.h"
#include "UART_HAL.h"
#include "Timer32_HAL.h"
///
//  Auxillary-to-Main Function Prototypes
///

// Initializes the UART module
void initialize(RPG* game);

// Draws the splash screen while blocking for three seconds
void splashScreen(Graphics_Context* g_sContext_p);

// Main menu - options to start game, high scores, and instructions
void mainMenu(RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx,
              unsigned vy);

// Driving while for the Dungeon Screen
void dungeonController(RPG* game, unsigned vx, unsigned vy, int monster_x,
                       int monster_y, int fire_x, int fire_y, int water_x,
                       int water_y, bool *water, bool *fire, int* monster_hp);

void battleController(RPG* game, unsigned vx, unsigned vy, bool water,
bool fire,
                      int monster_hp);

///
//  Main Function
///

int main(void)
{
    // Initialize board and game
    RPG game;
    button_t BoosterpackJoystickSelect = { GPIO_PORT_P4, GPIO_PIN1, Stable_R,
                                           RELEASED_STATE, &timer0 };
    initialize(&game);

    // vx and vy catch the joystick's 0th x-pos and y-pos bits
    unsigned vx = 0;
    unsigned vy = 0;

    // Opening splash screen. Blocks for three seconds.
    splashScreen(&game.g_sContext);

    // Main menu. Function contains a blocking while.
    mainMenu(&game, &BoosterpackJoystickSelect, vx, vy);
    getSampleJoyStick(&vx, &vy);

    // Display the Dungeon screen
    drawDungeonDisplay(&game.g_sContext, game.richterPos);

    // TODO: All enclosed will move to headers or auxillary functions.
    getSampleJoyStick(&vx, &vy);
    vx = vx % 5;
    vy = vy % 5;
    vx = 12 + vx * 25;
    vy = 12 + vy * 25;
    if (vx == 12 && vy == 12)
    {
        vx = 24;
        vy = 24;
    }
    Graphics_setForegroundColor(&game.g_sContext, GRAPHICS_COLOR_GREEN);
    Graphics_fillCircle(&game.g_sContext, vx, vy, 5);
    int monster_x = vx;
    int monster_y = vy;
    getSampleJoyStick(&vx, &vy);
    int monster_hp = 100;
    vx = vx % 5;
    vy = vy % 5;
    vx = 12 + vx * 25;
    vy = 12 + vy * 25;
    if (vx == 12 && vy == 12)
    {
        vx = 37;
        vy = 37;
    }
    if (vx == monster_x && vy == monster_y)
    {
        vx = vx + 25;
        vy = vy + 25;
    }
    Graphics_setForegroundColor(&game.g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_fillCircle(&game.g_sContext, vx, vy, 5);
    int water_x = vx;
    int water_y = vy;
    getSampleJoyStick(&vx, &vy);
    vx = vx % 5;
    vy = vy % 5;
    vx = 12 + vx * 25;
    vy = 12 + vy * 25;
    if (vx == 12 && vy == 12)
    {
        vx = 37;
        vy = 37;
    }
    if (vx == monster_x && vy == monster_y)
    {
        vx = vx + 25;
        vy = vy + 25;
    }
    if (vx == water_x && vy == water_y)
    {
        vx = vx + 25;
        vy = vy + 25;
    }
    int fire_x = vx;
    int fire_y = vy;
    Graphics_setForegroundColor(&game.g_sContext, GRAPHICS_COLOR_RED);
    Graphics_fillCircle(&game.g_sContext, vx, vy, 5);
    getSampleJoyStick(&vx, &vy);
    bool water = false;
    bool fire = false;
    // Player control begins - auxillary function dictates the Dungeon screen
    dungeonController(&game, vx, vy, monster_x, monster_y, water_x, water_y,
                      fire_x, fire_y, &water, &fire, &monster_hp);

    // Display battle screen
    drawBattleDisplay(&game.g_sContext, water, fire, monster_hp);

    // Player control begins - auxillary function dictates the Battle screen
    battleController(&game, vx, vy, water, fire, monster_hp);
}

///
//  Auxillary Functions
///

void initialize(RPG* game)
{
    // Stop the Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize the Hardware Timers
    initHWTimer0();
    initHWTimer1();

    eUSCI_UART_Config UART_Config = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // Configure UART to 9600bps for 48MHz clock and initialize
    UART_Config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK; // SMCLK Clock Source = 48MHz
    UART_Config.clockPrescalar = 312;                        // UCBR   = 312
    UART_Config.firstModReg = 8;                               // UCBRF  = 8
    UART_Config.secondModReg = 0x55;                        // UCBRS  = 0x55
    UART_Config.parity = EUSCI_A_UART_NO_PARITY,                // No Parity
    UART_Config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST,         // LSB First
    UART_Config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT, // One stop bit
    UART_Config.uartMode = EUSCI_A_UART_MODE,                   // UART mode
    UART_Config.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling
    InitUART(localModuleInstance, &UART_Config, GPIO_PORT_P1,
    GPIO_PIN2 | GPIO_PIN3);

    // ADC for Joystick
    initADC();
    initJoyStick();
    startADC();

    // Buttons
    initialize_BoosterpackJoystickSelect();

    // Display opening console text
    consoleIntro(localModuleInstance);

    // Initialize and draw game graphics
    initGraphics(&game->g_sContext);

    // Initialize game
    initGame(game);
}

void splashScreen(Graphics_Context* g_sContext_p)
{
    OneShotSWTimer_t myTimer;
    InitOneShotSWTimer(&myTimer, &timer0, 3000000);
    StartOneShotSWTimer(&myTimer);
    drawOpeningSplash(g_sContext_p);
    while (!OneShotSWTimerExpired(&myTimer))
        //delay for 3 seconds
        ;
}

#define OPTION1 0
#define OPTION3 2
void mainMenu(RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx,
              unsigned vy)
{
    Restart: ;
    int menuCursor = OPTION1;
    bool scoresBlock = false;
    bool helpBlock = false;
    uint8_t inputChar = '0';
    drawMainMenu(&game->g_sContext);
    while (true)
    {
        // Calculate RNG - might as well make use of this time~
        setupRandom(&game->randomSeed, vx, vy);

        // Discard characters
        if (UARTHasChar(localModuleInstance))
        {
            UARTGetChar(localModuleInstance);
        }

        // Block when in scores or instructions menu
        if (scoresBlock)
        {
            bool breakout = false;

            while (!breakout)
            {
                breakout = ButtonPushed(BoosterpackJoystickSelect);
            }

            goto Restart;
        }

        else if (helpBlock)
        {
            bool breakout = false;

            while (!breakout)
            {
                if (UARTHasChar(localModuleInstance))
                {
                    inputChar = UARTGetChar(localModuleInstance);
                    if (inputChar == '1')
                    {
                        consoleHelpDungeon(localModuleInstance);
                    }
                    else if (inputChar == '2')
                    {
                        consoleHelpBattle(localModuleInstance);
                    }
                }

                breakout = ButtonPushed(BoosterpackJoystickSelect);
            }

            goto Restart;
        }
        else
        {
            getSampleJoyStick(&vx, &vy);

            // Move cursor if not on first or last option, depending on position
            if (debouncedJoystickPushUp(vy) && menuCursor != OPTION1)
            {
                menuCursor--;
                drawMenuCursor(&game->g_sContext, menuCursor);

            }
            else if (debouncedJoystickPushDown(vy) && menuCursor != OPTION3)
            {
                menuCursor++;
                drawMenuCursor(&game->g_sContext, menuCursor);
            }

            if (ButtonPushed(BoosterpackJoystickSelect))
            {
                // Play the game!
                if (menuCursor == OPTION1)
                {
                    game->state = Dungeon;
                    return;
                }

                // How to Play menu
                else if (menuCursor == OPTION3)
                {
                    drawHelpScreen(&game->g_sContext);
                    helpBlock = true;
                }

                // View high scores
                else
                {
                    drawHighScores(&game->g_sContext, game->bestScores[0],
                                   game->bestScores[1], game->bestScores[2]);
                    scoresBlock = true;
                }
            }
        }
    }
}

#define UpLeftEdge 12
#define DownRightEdge 112
void dungeonController(RPG* game, unsigned vx, unsigned vy, int monster_x,
                       int monster_y, int fire_x, int fire_y, int water_x,
                       int water_y, bool *water, bool *fire, int *monster_hp)
{
    uint8_t inputChar;
    int counter = 1;
    //  bool fire = false;
    //bool water = false;
    while (game->state == Dungeon)
    {
        getSampleJoyStick(&vx, &vy);
        counter++;
        if (counter == 50000)
        {
            if (*monster_hp < 199)
                *monster_hp = *monster_hp + 1;
            counter = 0;
        }
        bool up = (inputChar == 'w') || debouncedJoystickPushUp(vy);
        bool down = (inputChar == 's') || debouncedJoystickPushDown(vy);
        bool right = (inputChar == 'd') || debouncedJoystickPushRight(vx);
        bool left = (inputChar == 'a') || debouncedJoystickPushLeft(vx);

        if (UARTHasChar(localModuleInstance))
        {
            inputChar = UARTGetChar(localModuleInstance);
        }

        if (game->state == Dungeon)
        {
            if (up && game->richterPos.y != UpLeftEdge)
            {
                inputChar = '0';

                moveRichterUp(&game->g_sContext, &game->richterPos);
            }
            if (down && game->richterPos.y != DownRightEdge)
            {
                inputChar = '0';

                moveRichterDown(&game->g_sContext, &game->richterPos);
            }
            if (right && game->richterPos.x != DownRightEdge)
            {
                inputChar = '0';

                moveRichterRight(&game->g_sContext, &game->richterPos);
            }
            if (left && game->richterPos.x != UpLeftEdge)
            {
                inputChar = '0';

                moveRichterLeft(&game->g_sContext, &game->richterPos);
            }

            if (game->richterPos.x == monster_x
                    && game->richterPos.y == monster_y)
            {
                game->state = Battle;
                clearScreen(&game->g_sContext);
            }
            if (game->richterPos.x == water_x && game->richterPos.y == water_y)
            {
                *water = true;
            }
            if (game->richterPos.x == fire_x && game->richterPos.y == fire_y)
            {
                *fire = true;
            }
        }
    }
}

void battleController(RPG* game, unsigned vx, unsigned vy, bool water,
bool fire,
                      int monster_hp)
{
    getSampleJoyStick(&vx, &vy);
    int player_hp = 100;
    bool monster_water = false;
    bool monster_fire = false;
    vx = vx % 3;
    switch (vx)
    {
    case 0:
        break;
    case 1:
        monster_water = true;
        break;
    case 2:
        monster_fire = true;
        break;
    }
    button_t JoystickButton;
    initButton(&JoystickButton, GPIO_PORT_P4, GPIO_PIN1, &timer1);
    uint8_t inputChar = '0';
    int cursor = OPTION1;
    while (game->state == Battle)
    {
        getSampleJoyStick(&vx, &vy);

        bool up = (inputChar == 'w') || debouncedJoystickPushUp(vy);
        bool down = (inputChar == 's') || debouncedJoystickPushDown(vy);

        if (up && cursor != OPTION1)
        {
            cursor--;
            drawBattleCursor(&game->g_sContext, cursor, water, fire);
        }
        if (down && cursor != OPTION3)
        {
            cursor++;
            drawBattleCursor(&game->g_sContext, cursor, water, fire);
        }
        if (ButtonPushed(&JoystickButton))
        {
            switch (cursor)
            {
            case 0:
                monster_hp = monster_hp - 15;
                break;
            case 1:
                if (water == true)
                {
                    if (monster_fire == true)
                    {
                        monster_hp = monster_hp - 40;
                    }
                    else
                    {
                        monster_hp = monster_hp - 20;
                    }
                }
                break;
            case 2:
                if (fire == true)
                {
                    if (monster_water == true)
                    {
                        monster_hp = monster_hp - 40;
                    }
                    else
                    {
                        monster_hp = monster_hp - 20;
                    }
                }
                break;
            }
            int8_t dracHealth[30] = "100 ";
            make_3digit_NumString(monster_hp, (char*) dracHealth);
            dracHealth[3] = '/';
            // dracHealth[4] = dracHealth[0];
            // dracHealth[5] = dracHealth[1];
            // dracHealth[6] = dracHealth[2];
            Graphics_setBackgroundColor(&game->g_sContext,
            GRAPHICS_COLOR_BLACK);
            Graphics_drawString(&game->g_sContext, dracHealth, -1, 65, 30,
            true);
            int i = 0;
            OneShotSWTimer_t myTimer;
            InitOneShotSWTimer(&myTimer, &timer0, 1000000);
            StartOneShotSWTimer(&myTimer);

            while (!OneShotSWTimerExpired(&myTimer))
                //delay for 3 seconds
                ;
            getSampleJoyStick(&vx, &vy);
            if (monster_hp != 0)
            {
                vy = vy % 5;
                switch (vy)
                {
                case 0:
                    player_hp = player_hp - 10;
                    break;
                case 1:
                    player_hp = player_hp - 15;
                    break;
                case 2:
                    player_hp = player_hp - 20;
                    break;
                case 3:
                    player_hp = player_hp - 25;
                    break;
                case 4:
                    player_hp = player_hp - 40;
                    break;
                }
                int8_t richHealth[30] = "100/100 ";
                make_3digit_NumString(player_hp, (char*) richHealth);
                Graphics_drawString(&game->g_sContext, richHealth, -1, 65, 70,
                                    true);
            }
        }
    }
}
