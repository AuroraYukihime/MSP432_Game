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
void mainMenu(RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx, unsigned vy);

// Driving while for the Dungeon Screen
void dungeonController(RPG* game, unsigned vx, unsigned vy);

///
//  Main Function
///

int main(void)
{
    // Initialize board and game
    RPG game;
    button_t BoosterpackJoystickSelect = {GPIO_PORT_P4, GPIO_PIN1, Stable_R, RELEASED_STATE, &timer0};
    initialize(&game);

    // vx and vy catch the joystick's 0th x-pos and y-pos bits
    unsigned vx = 0;
    unsigned vy = 0;

    // Opening splash screen. Blocks for three seconds.
    splashScreen(&game.g_sContext);

    // Main menu. Function contains a blocking while.
    mainMenu(&game, &BoosterpackJoystickSelect, vx, vy);

    // Display the Dungeon screen
    drawDungeonDisplay(&game.g_sContext, game.richterPos);
    dungeonController(&game, vx, vy);
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
void mainMenu(RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx, unsigned vy)
{
    int menuCursor = OPTION1;
    bool scoresBlock = false;
    bool helpBlock = false;
    uint8_t inputChar = '0';
    drawMainMenu(&game->g_sContext);

    while(true)
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

            while(!breakout)
            {
                breakout = ButtonPushed(BoosterpackJoystickSelect);
            }

            // Recursive call returns to menu
            mainMenu(game, BoosterpackJoystickSelect, vx, vy);
        }

        else if (helpBlock)
        {
            bool breakout = false;

            while(!breakout)
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

            // Recursive call returns to menu
            mainMenu(game, BoosterpackJoystickSelect, vx, vy);
        }
        else
        {
            getSampleJoyStick(&vx, &vy);

            // Move cursor if not on first or last option, depending on position
            if(debouncedJoystickPushUp(vy) && menuCursor != OPTION1)
            {
                menuCursor--;
                drawMenuCursor(&game->g_sContext, menuCursor);

            }
            else if(debouncedJoystickPushDown(vy) && menuCursor != OPTION3)
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
                    drawHighScores(&game->g_sContext, game->bestScores[0], game->bestScores[1], game->bestScores[2]);
                    scoresBlock = true;
                }
            }
        }
    }
}

void dungeonController(RPG* game, unsigned vx, unsigned vy)
{
    bool gameOver = false;
    uint8_t inputChar;
    while (!gameOver)
    {
        getSampleJoyStick(&vx, &vy);

        bool up    = (inputChar == 'w') || debouncedJoystickPushUp(vy);
        bool down  = (inputChar == 's') || debouncedJoystickPushDown(vy);
        bool right = (inputChar == 'd') || debouncedJoystickPushRight(vx);
        bool left  = (inputChar == 'a') || debouncedJoystickPushLeft(vx);

        // TODO: Move to UART Controller function
        if (UARTHasChar(localModuleInstance))
        {
            inputChar = UARTGetChar(localModuleInstance);
        }

        if (game->state == Dungeon)
        {
            if ( up && game->richterPos.y != 12)
            {
                inputChar = '0';

                moveRichterUp(&game->g_sContext, &game->richterPos);
            }
            if ( down && game->richterPos.y != 112)
            {
                inputChar = '0';

                moveRichterDown(&game->g_sContext, &game->richterPos);
            }
            if ( right && game->richterPos.x != 112)
            {
                inputChar = '0';

                moveRichterRight(&game->g_sContext, &game->richterPos);
            }
            if ( left && game->richterPos.x != 12)
            {
                inputChar = '0';

                moveRichterLeft(&game->g_sContext, &game->richterPos);
            }
        }
    }
}
