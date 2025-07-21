#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define AI0 PC0
#define PUMP_PIN PD2  // To-do: Allocate pins to peripherals


typedef enum State {  // New type for state machine
  IDLE,
  WATERING,
  LOGGING,
  ERROR
} state_t;


typedef struct {                         // Type to hold key characteristics of unique plants
  unsigned char species[10];             // String holding plant species name
  unsigned char sensorPin;               // Analog Input Pin for Plant's moisture sensor
  unsigned int moistureLevel;            // Most recently logged moisture level
  unsigned char targetMoistureRange[2];  // Indices define bounds of % range for ideal soil saturation
} Plant;


Plant hibiscus;  // Instantiation of Plant hibiscus

state_t currentState;  // Holds current system state

state_t sysInit();

void definePlants();

int main(void) {
  
  currentState = sysInit();  // Initialize system

  while (1) {  // State machine loop
    
    switch (currentState) {
      case IDLE:
        // sei();               // Enable global interrupts
        // sleep();             // Call AVR Sleep instruction
        break;
      case WATERING:
        // call start watering function
        // update state
        break;
      case LOGGING:
        // log desired data
        // update state
        break;
      case ERROR:
        // perform any fail safety actions
        // wait for error to clear
        // update state  
        break;
    }
  }
}


void definePlants() {                    // Defines attributes of existing Plants

  // Definition of Plant hibiscus
  strcpy(hibiscus.species, "hibiscus");  // Plant species assignment
  hibiscus.sensorPin = AI0;              // Moisture sensor input pin
  hibiscus.targetMoistureRange[0] = 50;  // Lower Bound
  hibiscus.targetMoistureRange[1] = 75;  // Upper Bound

  // Add any additional plants here
}


state_t sysInit() {
  
  SMCR |= (1 << SM1);       // Set Sleep Mode Control Register to Power Down Mode

  DDRD |= (1 << PUMP_PIN);  //Set pump pin data direction to output

  //Insert all pin configurations

  definePlants();

  //if something goes wrong, return ERROR;
  //else return LOGGING; // End initialization by entering LOGGING state to record startup values
}
