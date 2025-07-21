#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#define AI0 PC0
#define PUMP_PIN PD2  // To-do: Allocate pins to peripherals

SMCR |= (1 << SM1);  // Set Sleep Mode Control Register to Power Down Mode

typedef enum State {  // New type for state machine
  IDLE,
  INTERRUPT,
  WATERING,
  LOGGING,
  ERROR
} state_t;


typedef struct {                         // Type to hold key characteristics of unique plants
  unsigned char species[10];
  unsigned char sensorPin;
  unsigned int moistureLevel;
  unsigned char targetMoistureRange[2];  // Indices define bounds of % range for ideal soil saturation
} Plant;


Plant hibiscus;                        // Instantiation of Plant hibiscus
strcpy(hibiscus.species, "hibiscus");  // Plant species assignment
hibiscus.sensorPin = AI0;              // Moisture sensor input pin
hibiscus.targetMoistureRange[0] = 50;  // Lower Bound
hibiscus.targetMoistureRange[1] = 75;  // Upper Bound

state_t currentState;  // Holds current system state

state_t sysInit();

state_t sleep();

state_t wakeUp();

int main(void) {
  currentState = sysInit();  // Initialize system

  while (1) {  // State machine loop
    switch (currentState) {
      case IDLE:
        // SREG |= (1 << I);    // Enable global interrupts
        // sleep();             // Call AVR Sleep instruction
      case: INTERRUPT:
        // Decide what to do based on interrupt that occured
      case WATERING:
        // call start watering function
        // update state
      case LOGGING:
        // log desired data
        // update state
      case ERROR:
        // perform any fail safety actions
        // wait for error to clear
        // update state
        
    }
  
  }
  

}
