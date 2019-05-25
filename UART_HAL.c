#include "UART_HAL.h"

///
//  UART Initialization
///

void InitUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p, uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
    GPIO_setAsPeripheralModuleFunctionInputPin(selectedPort, selectedPins, GPIO_PRIMARY_MODULE_FUNCTION);
}

///
//  Change of Baud Rate
///

void UARTSetBaud(uint32_t moduleInstance, eUSCI_UART_Config *uartConfig_p, UARTBaudRate_t newBaud)
{
    switch(newBaud)
    {
    case baud9600:
        // Configure UART to 9600bps for 48MHz clock and initialize/enable
        uartConfig_p->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;           // SMCLK Clock Source = 48MHz
        uartConfig_p->clockPrescalar =  312;                                        // UCBR   = 312
        uartConfig_p->firstModReg = 8;                                              // UCBRF  = 8
        uartConfig_p->secondModReg = 0x55;                                          // UCBRS  = 0x55
        uartConfig_p->parity = EUSCI_A_UART_NO_PARITY,                              // No Parity
        uartConfig_p->msborLsbFirst = EUSCI_A_UART_LSB_FIRST,                       // LSB First
        uartConfig_p->numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,                 // One stop bit
        uartConfig_p->uartMode = EUSCI_A_UART_MODE,                                 // UART mode
        uartConfig_p->overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling

        // Maintain oversampling

        // Reinitialize and enable UART module
        UpdateUART(moduleInstance, uartConfig_p);
        break;
    case baud19200:
        // Configure UART to 19200bps for 48MHz clock and initialize/enable
        uartConfig_p->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;           // SMCLK Clock Source = 48MHz
        uartConfig_p->clockPrescalar =  156;                                        // UCBR   = 312
        uartConfig_p->firstModReg = 4;                                              // UCBRF  = 8
        uartConfig_p->secondModReg = 0x22;                                          // UCBRS  = 0x55
        uartConfig_p->parity = EUSCI_A_UART_NO_PARITY,                              // No Parity
        uartConfig_p->msborLsbFirst = EUSCI_A_UART_LSB_FIRST,                       // LSB First
        uartConfig_p->numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,                 // One stop bit
        uartConfig_p->uartMode = EUSCI_A_UART_MODE,                                 // UART mode
        uartConfig_p->overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling

        // Reinitialize and enable UART module
        UpdateUART(moduleInstance, uartConfig_p);
        break;
    case baud57600:
        // Configure UART to 57600bps for 48MHz clock and initialize/enable
        uartConfig_p->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;           // SMCLK Clock Source = 48MHz
        uartConfig_p->clockPrescalar =  52;                                        // UCBR   = 312
        uartConfig_p->firstModReg = 1;                                              // UCBRF  = 8
        uartConfig_p->secondModReg = 0x02;                                          // UCBRS  = 0x55
        uartConfig_p->parity = EUSCI_A_UART_NO_PARITY,                              // No Parity
        uartConfig_p->msborLsbFirst = EUSCI_A_UART_LSB_FIRST,                       // LSB First
        uartConfig_p->numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,                 // One stop bit
        uartConfig_p->uartMode = EUSCI_A_UART_MODE,                                 // UART mode
        uartConfig_p->overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; // Oversampling

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
    return (UART_getInterruptStatus(moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
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
    return (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
}

void UARTPutChar(uint32_t moduleInstance, uint8_t tChar)
{
    UART_transmitData(moduleInstance, tChar);
}
