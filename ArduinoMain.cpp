#include <stdio.h>
#include <avr/io.h>


typedef enum sysState {  // New type for state machine
  IDLE,
  WATERING,
  LOGGING,
  ERROR
} state_t;


state_t currentState;  // Holds current system state

state_t sysInit();



int main(void) {
  currentState = sysInit();  // Initialize system

  while (1) {  // State machine loop
    switch (currentState) {
      case IDLE:
        // wait
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
