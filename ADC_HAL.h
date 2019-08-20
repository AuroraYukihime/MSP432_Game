#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include "Graphics_HAL.h" // need bool plz

#define DOWN_THRESHOLD    1000
#define LEFT_THRESHOLD  0x0500
#define UP_THRESHOLD     15000
#define RIGHT_THRESHOLD 0x3500

#define UP_HIGH          14000
#define UP_MID           11000
#define RETURN_MID       11500
#define RETURN_LOW        8500

#define RIGHT_HIGH       14000
#define RIGHT_MID        11000
#define RETURN_MID       11500
#define RETURN_LOW        8500

#define LEFT_75           1500
#define LEFT_80           3000
#define LEFT_85           4500
#define RIGHT_95         10500
#define RIGHT_100        12000
#define RIGHT_105        13500

#define LEFT_CENTER       7250
#define DOWN_CENTER       7750
#define UP_CENTER         8250
#define RIGHT_CENTER      8750

typedef struct
{
    unsigned int ones;
    unsigned int zeroes;

    // Manages the array of random bits for randomization
    unsigned int topFive[5];
    unsigned int count;
} counterType;

void initADC();
void startADC();
void initJoyStick();
void getSampleJoyStick(unsigned *X, unsigned *Y);

// Finite state machine for debounced joystick.
// Inputs: vx and vy of current joystick position
// Outputs: Returns true if joystick is tapped (not held) up/down. Will only return true once per entry/exit past the threshold.
bool debouncedJoystickPushUp(unsigned y);
bool debouncedJoystickPushDown(unsigned y);

void setupRandom(counterType* counter, unsigned x, unsigned y);
unsigned int generateRandom(counterType * counter, unsigned x, unsigned y);

#endif /* ADC_HAL_H_ */
