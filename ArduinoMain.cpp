#include <stdio.h>
#include <avr/io.h>

#define PUMP_PIN ...  // To-do: Allocate pins to peripherals
#define ...

SMCR |= (1 << SM1);  // Set Sleep Mode Control Register to Power Down Mode

typedef enum sysState {  // New type for state machine
  IDLE,
  INTERRUPT,
  WATERING,
  LOGGING,
  ERROR
} state_t;


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
