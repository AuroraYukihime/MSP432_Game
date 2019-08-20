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
void initialize(Graphics_Context* g_sContext_p, eUSCI_UART_Config UART_Config, RPG* game);

// Draws the splash screen while blocking for three seconds
void splashScreen(Graphics_Context* g_sContext_p);

// Main menu - options to start game, high scores, and instructions
void mainMenu(Graphics_Context* g_sContext_p, RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx, unsigned vy);

// TBD
void clear_player(Graphics_Context* g_sContext_p, int x, int y, bool current);

///
//  Main Function
///

int main(void)
{
    // Initialize board and game
    RPG game;
    Graphics_Context g_sContext;
    eUSCI_UART_Config UART_Config = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    initialize(&g_sContext, UART_Config, &game);

    button_t BoosterpackJoystickSelect = {GPIO_PORT_P4, GPIO_PIN1, Stable_R, RELEASED_STATE, &timer0};

    // vx and vy catch the joystick's 0th x-pos and y-pos bits
    unsigned vx = 0;
    unsigned vy = 0;

    // Draw opening splash
    splashScreen(&g_sContext);

    mainMenu(&g_sContext, &game, &BoosterpackJoystickSelect, vx, vy);

    Graphics_clearDisplay(&g_sContext);
    drawGameDisplay(&g_sContext);

    // Driving while loop w/ UART controller
    bool gameOver = false;
    bool gameStart = false;
    int x = 8;
    int y = 120;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(&g_sContext, x, y, 3);
    uint8_t bruh;
    bool maroon = true;
    while (!gameOver)
    {
        if (UARTHasChar(localModuleInstance))
        {
            bruh = UARTGetChar(localModuleInstance);
        }
        if (!gameStart)
        {
            if (bruh == 'h')
            {
                bruh = '0';

                consoleHelp(localModuleInstance);
            }
            if (bruh == 's')
            {
                bruh = '0';
                game.state = Dungeon;
                gameStart = true;
            }
        }
        if (game.state == Dungeon && gameStart)
        {
            if (bruh == 'h')
            {
                bruh = '0';

                consoleHelp_2(localModuleInstance);
            }
            if ((bruh == 'w') && y != 8)
            {
                bruh = '0';
                // drawGameDisplay(&g_sContext);
                clear_player(&g_sContext, x, y, maroon);
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                y = y - 16;
                Graphics_fillCircle(&g_sContext, x, y, 3);
                if (maroon == true)
                    maroon = false;
                else
                    maroon = true;
            }
            if ((bruh == 's') && y != 120)
            {
                bruh = '0';
                //drawGameDisplay(&g_sContext);
                clear_player(&g_sContext, x, y, maroon);
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                y = y + 16;
                Graphics_fillCircle(&g_sContext, x, y, 3);
                if (maroon == true)
                    maroon = false;
                else
                    maroon = true;
            }
            if (bruh == 'd' && x != 120)
            {
                bruh = '0';
                //drawGameDisplay(&g_sContext);
                clear_player(&g_sContext, x, y, maroon);
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                x = x + 16;
                Graphics_fillCircle(&g_sContext, x, y, 3);
                if (maroon == true)
                    maroon = false;
                else
                    maroon = true;
            }
            if (bruh == 'a' && x != 8)
            {
                bruh = '0';
                //drawGameDisplay(&g_sContext);

                clear_player(&g_sContext, x, y, maroon);
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                x = x - 16;
                Graphics_fillCircle(&g_sContext, x, y, 3);
                if (maroon == true)
                    maroon = false;
                else
                    maroon = true;
            }
        }
    }
}

///
//  Auxillary Functions
///

void initialize(Graphics_Context* g_sContext_p, eUSCI_UART_Config UART_Config, RPG* game)
{
    // Stop the Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize the Hardware Timers
    initHWTimer0();
    initHWTimer1();

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
    initGraphics(g_sContext_p);

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
void mainMenu(Graphics_Context* g_sContext_p, RPG* game, button_t* BoosterpackJoystickSelect, unsigned vx, unsigned vy)
{
Restart:;
    int menuCursor = OPTION1;
    bool scoresBlock = false;
    bool helpBlock = false;

    drawMainMenu(g_sContext_p);

    while(true)
    {
        // Calculate RNG - might as well make use of this time~
        setupRandom(&game->randomSeed, vx, vy);

        // Block when in scores or instructions menu
        if (scoresBlock || helpBlock)
        {
            bool breakout = false;
            while(!breakout)
            {
                breakout = ButtonPushed(BoosterpackJoystickSelect);
            }
            // Restart menu
            goto Restart;
        }

        getSampleJoyStick(&vx, &vy);

        // Move cursor if not on first or last option, depending on position
        if(debouncedJoystickPushUp(vy) && menuCursor != OPTION1)
        {
            menuCursor--;
            drawMenuCursor(g_sContext_p, menuCursor);

        }
        else if(debouncedJoystickPushDown(vy) && menuCursor != OPTION3)
        {
            menuCursor++;
            drawMenuCursor(g_sContext_p, menuCursor);
        }

        if (ButtonPushed(BoosterpackJoystickSelect))
        {
            // Play the game!
            if (menuCursor == OPTION1)
            {
                return;
            }

            // How to Play menu
            else if (menuCursor == OPTION3)
            {
                drawHelpScreen(g_sContext_p);
                helpBlock = true;
            }

            // View high scores
            else
            {
                drawHighScores(g_sContext_p, game->bestScores[0], game->bestScores[1], game->bestScores[2]);
                scoresBlock = true;
            }
        }
    }
}

void clear_player(Graphics_Context* g_sContext_p, int x, int y, bool current)
{
    if (current == true)
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_MAROON);
        Graphics_fillCircle(g_sContext_p, x, y, 3);
    }
    else
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
        Graphics_fillCircle(g_sContext_p, x, y, 3);
    }
}
