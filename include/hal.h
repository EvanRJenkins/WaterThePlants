#ifndef HAL_H
#define HAL_H

#include "main.h"
#include <avr/io.h>

/* Pin aliases (Arduino Uno R3) */

#define AI0 PC0
#define AI1 PC1
#define AI2 PC2
#define AI3 PC3
#define AI4 PC4
#define AI5 PC5
#define PUMP_PIN PD2  // To-do: Allocate pins to peripherals


/* Function Prototypes */

void regConfig();
state_t adcRecord(uint8_t targetPin);
state_t startPump();
state_t stopPump();


#endif // HAL_H
