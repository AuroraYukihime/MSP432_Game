#include "UART_HAL.h"

///
//  UART Initialization
///

void InitUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p,
              uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
    GPIO_setAsPeripheralModuleFunctionInputPin(selectedPort, selectedPins,
    GPIO_PRIMARY_MODULE_FUNCTION);
}

///
//  Change of Baud Rate
///

void UARTSetBaud(uint32_t moduleInstance, eUSCI_UART_Config *uartConfig_p,
                 UARTBaudRate_t newBaud)
{
    switch (newBaud)
    {
    case P1_9600:
        // Configure UART to 9600bps for 48MHz clock
        uartConfig_p->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK; // SMCLK Clock Source = 48MHz
        uartConfig_p->clockPrescalar = 312;                      // UCBR   = 312
        uartConfig_p->firstModReg = 8;                             // UCBRF  = 8
        uartConfig_p->secondModReg = 0x55;                      // UCBRS  = 0x55
        uartConfig_p->parity = EUSCI_A_UART_NO_PARITY,              // No Parity
        uartConfig_p->msborLsbFirst = EUSCI_A_UART_LSB_FIRST,       // LSB First
        uartConfig_p->numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT, // One stop bit
        uartConfig_p->uartMode = EUSCI_A_UART_MODE,                 // UART mode
        uartConfig_p->overSampling =
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling

        // Reinitialize and enable UART module
        UpdateUART(moduleInstance, uartConfig_p);
        break;
    case P2_19200:
        // Configure UART to 19200bps for 48MHz clock
        uartConfig_p->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK; // SMCLK Clock Source = 48MHz
        uartConfig_p->clockPrescalar = 156;                      // UCBR   = 312
        uartConfig_p->firstModReg = 4;                             // UCBRF  = 8
        uartConfig_p->secondModReg = 0x22;                      // UCBRS  = 0x55
        uartConfig_p->parity = EUSCI_A_UART_NO_PARITY,              // No Parity
        uartConfig_p->msborLsbFirst = EUSCI_A_UART_LSB_FIRST,       // LSB First
        uartConfig_p->numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT, // One stop bit
        uartConfig_p->uartMode = EUSCI_A_UART_MODE,                 // UART mode
        uartConfig_p->overSampling =
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling

        // Reinitialize and enable UART module
        UpdateUART(moduleInstance, uartConfig_p);
        break;
    }
}

void UpdateUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
}

///
//  Act as Receiver
///

bool UARTHasChar(uint32_t moduleInstance)
{
    return (UART_getInterruptStatus(moduleInstance,
    EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
}

uint8_t UARTGetChar(uint32_t moduleInstance)
{
    return UART_receiveData(moduleInstance);
}

///
//  Act as Transmitter
///

bool UARTCanSend(uint32_t moduleInstance)
{
    return (UART_getInterruptStatus(EUSCI_A0_BASE,
    EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
            == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
}

void UARTPutChar(uint32_t moduleInstance, uint8_t tChar)
{
    UART_transmitData(moduleInstance, tChar);
}

///
//  Game Console
///

// Helper Function
// Writes strings to UART console
void writeString(uint32_t moduleInstance, string_t string, unsigned int size)
{
    int stringIndex = 0;
    bool complete = false;
    while (!complete)
    {
        if (UARTCanSend(moduleInstance))
        {
            UARTPutChar(moduleInstance, string.string[stringIndex]);
            stringIndex++;
        }

        if (stringIndex == size)
            complete = true;
    }
}

#define intro1Size 22
#define intro2Size 28
#define intro3size 39
#define intro4size 46
void consoleIntro(uint32_t moduleInstance)
{
    string_t string;

    string.string = "Just Monika Presents\r\n";
    writeString(moduleInstance, string, intro1Size);

    string.string = "Placeholder RPG for MSP432\r\n";
    writeString(moduleInstance, string, intro2Size);

    string.string = "Press h for instructions and controls\r\n";
    writeString(moduleInstance, string, intro3size);

    string.string = "Otherwise press any other key for game start\r\n";
    writeString(moduleInstance, string, intro4size);

    string.string = "\n";
    writeString(moduleInstance, string, spacerSize);
}

#define help1Size 26
#define help2Size 62
#define help3Size 59
#define help4Size 50
#define help5Size 63
void consoleHelp(uint32_t moduleInstance)
{
    string_t string;

    string.string = "Player 1 is on 9600bps.\r\n";
    writeString(moduleInstance, string, help1Size);

    string.string =
            "Move using joystick. Keep moving until you find the monster.\r\n";
    writeString(moduleInstance, string, help2Size);

    string.string =
            "Get to the same square as the monster to iniate a battle.\r\n";
    writeString(moduleInstance, string, help3Size);

    string.string = "Reference manual for additional rules in effect.\r\n";
    writeString(moduleInstance, string, help4Size);

    string.string = "\n";
    writeString(moduleInstance, string, spacerSize);
}
void consoleHelp_2(uint32_t moduleInstance)
{
    string_t string;

    string.string = "Player 1 is on 9600bps.\r\n";
    writeString(moduleInstance, string, help1Size);

    string.string =
            "Move using joystick. Keep moving until you find the monster.\r\n";
    writeString(moduleInstance, string, help2Size);

    string.string =
            "Get to the same square as the monster to iniate a battle.\r\n";
    writeString(moduleInstance, string, help3Size);

    string.string = "The Monster gets stronger every 1 second until you get to it.\r\n";
    writeString(moduleInstance, string, help4Size);

    string.string = "\n";
    writeString(moduleInstance, string, spacerSize);
}
