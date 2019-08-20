#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "ADC_HAL.h"

// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    // This sets the conversion clock to 3MHz
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 up to ADC_MEM1. Each conversion will
    // thus use two channels.
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM1,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9,                 // joystick Y
                                    ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A9 is multiplexed on GPIO port P4 pin PIN4
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
}

void getSampleJoyStick(unsigned *X, unsigned *Y) {
    // ADC runs in continuous mode, we just read the conversion buffers
    *X = ADC14_getResult(ADC_MEM0);
    *Y = ADC14_getResult(ADC_MEM1);
}

bool debouncedJoystickPushUp(unsigned y)
{
    // Subactive = Joystick is not past the threshold.
    // Active = Joystick was subactive last cycle and is past the right threshold now!
    // Saturation = Joystick is held past the threshold, but will return false as per project spec.
    static enum {Subactive, Active, Saturation} state;

    bool returnStatus = false;

    switch(state)
    {
    case Subactive:
        if (y >= UP_THRESHOLD)
        {
            state = Active;
        }
        break;
    case Active:
        if (y >= UP_CENTER)
        {
            state = Saturation;
        }
        else state = Subactive;

        break;
    case Saturation:
        if (y <= UP_CENTER)
        {
            state = Subactive;
            returnStatus = true;
        }
        break;
    }

    return returnStatus;
}

bool debouncedJoystickPushDown(unsigned y)
{
    // Subactive = Joystick is not past the threshold.
    // Active = Joystick was subactive last cycle and is past the right threshold now!
    // Saturation = Joystick is held past the threshold, but will return false as per project spec.
    static enum {Subactive, Active, Saturation} state;

    bool returnStatus = false;

    switch(state)
    {
    case Subactive:
        if (y <= DOWN_THRESHOLD)
        {
            state = Active;
        }
        break;
    case Active:
        if (y <= DOWN_CENTER)
        {
            state = Saturation;
        }
        else state = Subactive;

        break;
    case Saturation:
        if (y >= DOWN_CENTER)
        {
            state = Subactive;
            returnStatus = true;
        }
        break;
    }

    return returnStatus;
}


// Helper to setupRandom
// Returns the toggle (XOR) of raw vx and vy joystick input
bool createRandomBit(unsigned vx, unsigned vy)
{
    return ((vx & BIT0) ^ (vy & BIT0));
}

#define MAX_COUNT 5
// Helper to generateRandom
// Produces ones and zero counts of the LSBs of the joystick coordinates
void setupRandom(counterType* counter, unsigned x, unsigned y)
{
    if (createRandomBit(x, y))
    {
        counter->ones++;
        counter->topFive[counter->count] = 1;
    }
    else
    {
        counter->zeroes++;
        counter->topFive[counter->count] = 0;
    }

    counter->count++;
    if (counter->count == MAX_COUNT) counter->count = 0;
}

unsigned int generateRandom(counterType * counter, unsigned x, unsigned y)
{
    unsigned int returnMe = 0;

    // Odds are we'll likely get zero sometimes
    // Handler below handles this situation to give a pin number we can display.
    static int zeroOccurrences = 0;

    if (counter->topFive[0] == 1) returnMe |= BIT3;
    if (counter->topFive[1] == 1) returnMe |= BIT2;
    if (counter->topFive[2] == 1) returnMe |= BIT1;
    if (counter->topFive[3] == 1) returnMe |= BIT0;

    if (returnMe > 10)
    {
        returnMe %= 10;
    }

    // Zero handler
    if (returnMe == 0)
    {
        returnMe = 1 + zeroOccurrences;
    }

    return returnMe;
}
