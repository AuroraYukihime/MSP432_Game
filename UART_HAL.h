#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
typedef enum {baud9600, baud19200, baud57600} UARTBaudRate_t;

///
//  Initialization
///

// Initializes and enables UART module; designates the input pins in UART mode.
void InitUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p, uint_fast8_t selectedPort, uint_fast16_t selectedPins);

///
//  Change of Baud Rate
///

// Changes the UART config struct values to suit a new baud rate, then pushes to UpdateUART
void UARTSetBaud(uint32_t moduleInstance, eUSCI_UART_Config *uartConfig_p, UARTBaudRate_t newBaud);

// Reinitializes and enables the UART module for a change in baud rate
void UpdateUART(uint32_t moduleInstance, const eUSCI_UART_Config *uartConfig_p);

///
//  Act as Receiver
///

// Checks the UART module's interrupt status for user input
// Indicates that UART can receive a character from the console
bool UARTHasChar(uint32_t moduleInstance);

// Returns the received data from the UART module. For our case, this is the user input character.
uint8_t UARTGetChar(uint32_t moduleInstance);

///
//  Act as Transmitter
///

// Checks the UART module's transmit interrupt status, returns true if present
// Indicates that UART can send a character to console
bool UARTCanSend(uint32_t moduleInstance);

// Transmits a character to the MobaXTerm console
void UARTPutChar(uint32_t moduleInstance, uint8_t tChar);
