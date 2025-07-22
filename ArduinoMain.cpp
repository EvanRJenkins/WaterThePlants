#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PLANTS_IN_GARDEN 2  // Adjust depending on # of plants currently in garden
#define AI0 PC0
#define AI1 PC1
#define AI2 PC2
#define AI3 PC3
#define AI4 PC4
#define AI5 PC5
#define PUMP_PIN PD2  // To-do: Allocate pins to peripherals


/* ISR Handler Macros */

ISR (PCINT0_vect) {  // Define Pin change interrupt handler for pin PB0 (D8)
  current = LOG_PRE;
}

ISR (ADC_vect) {  // Define ADC conversion complete interrupt handler
  update next plant pointer;
}


/* Type Definitions */

typedef enum State {  // Enum for state machine
  IDLE,               // Power down, only exit via interrupt
  LOG_PRE,            // Log before watering
  WATERING,           // Send water to target plant(s)
  LOG_POST,           // Log after watering
  ERROR               // System fault, block all tasks until error ack
} state_t;

typedef struct {                         // Holds key characteristics of an individual plant
  unsigned char species[10];             // String holding plant species name
  unsigned char sensorPin;               // Analog Input Pin for Plant's moisture sensor
  unsigned int moistureLevel;            // Most recently logged moisture level
  unsigned char targetMoistureRange[2];  // Indices define bounds of % range for ideal soil saturation
} Plant;


/* Global Variables */

state_t currentState;  // Holds current system state

Plant plantList[PLANTS_IN_GARDEN];  // Array of all plants


/* Startup functions */

state_t sysInit();     // Initializes registers and variables

void definePlants();   // Subroutine of sysInit for Plant definitions

void regConfig();      // Subroutine of sysInit for Register configurations




int main(void) {
  
  currentState = sysInit();  // Initialize system

  while (1) {  // State machine loop
    
    switch (currentState) {
      case IDLE:
        // sei();               // Enable global interrupts
        // sleep();             // Call AVR Sleep instruction
        break;
      case LOG_PRE:
        // log desired data
          // start ADC
          // read ADCL and ADCH and copy to Plant.MoistureLevel
        // update state
        break;
      case WATERING:
        // adjust watering sequence
        // water target plant(s)
        // update state
        break;
      case LOG_POST:
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


void definePlants() {

/* Definition of plants in garden */

  strcpy(plantList[0].species, "hibiscus");     // Define plantList[0] as "hibiscus"
  plantList[0].sensorPin = AI0;                 // Moisture sensor input pin
  plantList[0].targetMoistureRange[0] = 50;     // Lower Bound
  plantList[0].targetMoistureRange[1] = 75;     // Upper Bound

  strcpy(plantList[1].species, "hibiscus");     // Define plantList[1] as "marigold"
  plantList[1].sensorPin = AI1;                 // Moisture sensor input pin
  plantList[1].targetMoistureRange[0] = 50;     // Lower Bound
  plantList[1].targetMoistureRange[1] = 75;     // Upper Bound
  
  // Add any additional plants here
}

void regConfig() {
  
  DDRB &= ~(1 << DDB0);     // Set pin D8 as input
  PORTB |= (1 << PORTB0);   // Enable pull-up on pin D8
  SMCR |= (1 << SM1);       // Set Sleep Mode Control Register to Power Down Mode
  DDRD |= (1 << PUMP_PIN);  // Set pump pin data direction to output
  ADCSRA |= (1 << ADIE);    // Enable 'ADC complete' interrupt
  
}

state_t sysInit() {
  
  regConfig();
  
  definePlants();

  //if something goes wrong, return ERROR;
  //else  call sei(); return IDLE; // End initialization by entering IDLE state to wait for PCINT0
}
