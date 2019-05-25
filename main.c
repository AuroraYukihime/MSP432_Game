// System Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Module Includes
#include "UART_HAL.h"

///
//  Auxillary-to-Main Function Prototypes
///

// Initializes the UART module
void initialize(eUSCI_UART_Config UART_Config);

///
//  Main Function
///

int main(void)
{
    // Initialize UART connection
    eUSCI_UART_Config UART_Config = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    initialize(UART_Config);

    consoleIntro(localModuleInstance);
}

///
//  Auxillary to Main Functions
///

void initialize(eUSCI_UART_Config UART_Config)
{
    // Stop the Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    // Configure UART to 9600bps for 48MHz clock and initialize
    UART_Config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;             // SMCLK Clock Source = 48MHz
    UART_Config.clockPrescalar =  312;                                          // UCBR   = 312
    UART_Config.firstModReg = 8;                                                // UCBRF  = 8
    UART_Config.secondModReg = 0x55;                                            // UCBRS  = 0x55
    UART_Config.parity = EUSCI_A_UART_NO_PARITY,                                // No Parity
    UART_Config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST,                         // LSB First
    UART_Config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,                   // One stop bit
    UART_Config.uartMode = EUSCI_A_UART_MODE,                                   // UART mode
    UART_Config.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;   // Oversampling
    InitUART(localModuleInstance, &UART_Config, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
}
