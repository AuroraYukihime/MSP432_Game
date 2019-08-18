// System Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Module Includes
#include "Graphics_HAL.h"
#include "UART_HAL.h"
#include "Timer32_HAL.h"
///
//  Auxillary-to-Main Function Prototypes
///
typedef enum
{
    INFO, GAME, FIGHT
} gamestate;
// Initializes the UART module
void initialize(Graphics_Context* g_sContext_p, eUSCI_UART_Config UART_Config);

///
//  Main Function
///

int main(void)
{
    // Initialize UART connection
    Graphics_Context g_sContext;
    eUSCI_UART_Config UART_Config = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    initialize(&g_sContext, UART_Config);
    gamestate current = INFO;
    // Draw opening splash
    // TODO: Block with music
    extern HWTimer_t timer0, timer1;
    initHWTimer0();
    initHWTimer1();
    OneShotSWTimer_t myTimer;
    InitOneShotSWTimer(&myTimer, &timer0, 3000000);
    StartOneShotSWTimer(&myTimer);
    drawOpeningSplash(&g_sContext);
    while (!OneShotSWTimerExpired(&myTimer))
        //delay for 3 seconds
        ;

    Graphics_clearDisplay(&g_sContext);
    drawGameDisplay(&g_sContext);

    // Driving while loop w/ UART controller
    bool gameOver = false;
    bool gameStart = false;
    bool move = false;
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
                current = GAME;
                gameStart = true;
            }
        }
        if (current == GAME && gameStart)
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
//  Auxillary to Main Functions
///

void initialize(Graphics_Context* g_sContext_p, eUSCI_UART_Config UART_Config)
{
    // Stop the Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

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

    // Display opening console text
    consoleIntro(localModuleInstance);

    // Initialize and draw game graphics
    initGraphics(g_sContext_p);
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
