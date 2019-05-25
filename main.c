// System Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Module Includes
#include "UART_HAL.h"

// ModuleInstance for UART Protocol
#define moduleInstance EUSCI_A0_BASE

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
    eUSCI_UART_Config UART_Config;
    initialize(UART_Config);

    while(1)
    {
        // Temporary block
        // Source: ECE-2534, Spring 2019 with Dr. Nazhand-Ali (Homework 6 UART Starter Code)
        char rChar, tChar;
        if (UARTHasChar(EUSCI_A0_BASE))
        {
            rChar = UARTGetChar(EUSCI_A0_BASE);

            // Depending on if the received char is a Number, a Letter, or Otherwise,
            // the transmit char is N, L or O
            if (('0'<=rChar) && (rChar <= '9'))
                tChar = 'N';
            else if ((('a'<=rChar) && (rChar <= 'z')) ||
                    (('A'<=rChar) && (rChar <= 'Z')))
                tChar = 'L';
            else
                tChar = 'O';

            if (UARTCanSend(EUSCI_A0_BASE))
                UARTPutChar(EUSCI_A0_BASE, tChar);
        }
    }
}

///
//  Auxillary to Main Functions
///

void initialize(eUSCI_UART_Config UART_Config)
{
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
    InitUART(moduleInstance, &UART_Config, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
}
