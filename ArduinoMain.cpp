#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* Interrupt bitfield bits */

#define PCINT0 (1 << 0)  // Bit 0 is PCINT0
#define ADC (1 << 1)     // Bit 1 is ADC complete
#define WTRDONE (1 << 2) // Bit 2 is watering done

/* Constants dependent of physical setup */

#define PLANTS_IN_GARDEN 2  // Adjust depending on # of plants currently in garden


/* Pin aliases (Arduino Uno R3) */

#define AI0 PC0
#define AI1 PC1
#define AI2 PC2
#define AI3 PC3
#define AI4 PC4
#define AI5 PC5
#define PUMP_PIN PD2  // To-do: Allocate pins to peripherals


/* ISR Handler Macros */

ISR (PCINT0_vect) {   // Interrupt handler for pin change at PB0 (D8)
  
  g_isrFlags |= PCINT0;

}

ISR (ADC_vect) {      // Interrupt handler for ADC conversion complete
  g_isrFlags |= ADC;  // Flag new data availiable
}

ISR (//custom watering done interrupt) {      // Interrupt handler for WTRDONE
  g_isrFlags |= WTRDONE;  // Flag new data availiable
}


/* Type Definitions */

typedef enum State {  // Enum for state machine
  IDLE,               // Power down, only exit via interrupt
  LOG_PRE,            // Record moisture of currentPlantIndex Plant
  WAITING_FOR_ADC,    // Wait for new data, increment currentPlantIndex, go to LOG_PRE
  START_WATERING,     // Send water to all plants
  WAIT_FOR_FLOW,      // Delay until flow checkpoint is reached, ERROR after timeout
  WATERING,           // Water flowing to plants
  STOP_WATERING,      // Stop watering, transition to LOG_POST
  LOG_POST,           // Log after watering
  ERROR               // System fault, block all tasks until error ack
} state_t;


typedef struct {                   // Holds key characteristics of an individual plant
  unsigned char species[10];       // String holding plant species name
  uint8_t sensorPin;               // Analog Input Pin for Plant's moisture sensor
  uint16_t moistureLevel;          // Most recently logged moisture level
} Plant;


/* Global variables */

volatile uint8_t g_isrFlags = 0;  // Bitfield holding all ISR flags

state_t currentState;  // Holds current system state

Plant plantList[PLANTS_IN_GARDEN];  // Array of all plants

uint8_t currentPlantIndex;


/* Startup functions */

state_t sysInit();     // Initializes registers and variables

void definePlants();   // Subfunction of sysInit for Plant definitions

void regConfig();      // Subfunction of sysInit for Register configurations


/* State Functions */

state_t adcRecord(uint8_t targetPin);

state_t startPump();

state_t stopPump();


/* Main */

int main(void) {
  
  currentState = sysInit();  // Initialize system

  while (1) {                // State machine loop
    switch (currentState) {


      case IDLE:  

        if (g_isrFlags & PCINT0) {
          currentState = LOG_PRE;
          g_isrFlags &= ~PCINT0;
        }
          
        else {
          sleep();             // Call AVR Sleep instruction
        }
          
        break;

      
      case LOG_PRE:
        if (currentPlantIndex < PLANTS_IN_GARDEN) {
          currentState = adcRecord(plantList[currentPlantIndex].sensorPin);
        }
        
        else {
          currentState = START_WATERING;
          currentPlantIndex = 0;
        }
        break;


      case WAITING_FOR_ADC:

        if ((g_isrFlags & ADC)) {    // Evaluates as true if ADC flag bit is set
          plantList[currentPlantIndex].moistureLevel = ADCW;  // Load ADC data to current plant moistureLevel
          ++currentPlantIndex;
          g_isrFlags &= ~ADC;
          if ((g_isrFlags & ~(WTRDONE)) {
            currentState = LOG_PRE;
          }
          else {
            currentState = LOG_POST;
          }
        }
        
        break;  // Stay here until ADC complete interrupt


      case START_WATERING:

        currentState = startPump();
        
        break;


      case WAIT_FOR_FLOW:

        if (flowCheckpoint) {
          currentState = WATERING;
        }
        
        break;
      

      case WATERING:

        //start timer

        if (g_isrFlags & WTRDONE) {
          currentState = STOP_WATERING;
        }
        break;


      case STOP_WATERING:

        currentState = stopPump();

        break;
      
      case LOG_POST:
        
         if (currentPlantIndex < PLANTS_IN_GARDEN) {
          currentState = adcRecord(plantList[currentPlantIndex].sensorPin);
        }
        
        else {
          g_isrFlags = 0x0;
          currentPlantIndex = 0;
          currentState = IDLE;
        }
        
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
  
  strcpy(plantList[1].species, "hibiscus");     // Define plantList[1] as "marigold"
  plantList[1].sensorPin = AI1;                 // Moisture sensor input pin
  
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
  
  sei(); 
  
  return IDLE;
}


state_t adcRecord(uint8_t targetPin) {    // Enables ADC an starts conversion for targetPin

/* Set MUX2..0 to ADC channel of targetPin */
 switch (targetPin) {
  
   case AI0:  // 000
    ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    break;
  
   case AI1:  // 001
    ADMUX &= ~((1 << MUX2) | (1 << MUX1));
    ADMUX |= (1 << MUX0);
    break;
  
   case AI2:  // 010
    ADMUX &= ~((1 << MUX2) | (1 << MUX0));
    ADMUX |= (1 << MUX1);
    break;
  
   case AI3:  // 011
    ADMUX &= ~(1 << MUX2);
    ADMUX |= (1 << MUX1) | (1 << MUX0);
    break;
  
   case AI4:  // 100
    ADMUX &= ~((1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX2);
    break;
  
   case AI5:  // 101
    ADMUX &= ~(1 << MUX1);
    ADMUX |= (1 << MUX2) | (1 << MUX0);
    break;
  
   default: // Default to channel 0
    ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    break;
 }


  ADCSRA |= (1 << ADEN);  // Enable ADC (Set ADEN)
  ADCSRA |= (1 << ADSC);  // Start ADC conversion (Set ADSC bit)

  return WAITING_FOR_ADC;
}


state_t startPump() {
  
  PORTD |= PUMP_PIN;  // Activate tank pump  
  
  return WAIT_FOR_FLOW;
}


state_t stopPump() {
  
  PORTD &= ~(PUMP_PIN);  // Deactivate tank pump  
  
  return LOG_POST;
}
