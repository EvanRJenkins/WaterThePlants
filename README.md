# **water_the_plants**
*Automated Watering System Firmware for Potted Plants*

*This is firmware for an automated plant watering system using an AVR microcontroller. It uses a state machine to check soil moisture and control a water tank pump.*

## **System Features**

**Deterministic Design**: Operates as an interrupt-driven state machine.
**Portability**: Can be configured for different garden setups and plants species.
**Fail Safety**: Non-blocking architecture with descriptive alerts and timeout conditions.
**Customizable Controls**: Can be triggered via timer or manual input.
**Power Efficiency**: Sleeps until watering protocol is activated.

## **Hardware Requirements**

  ### MCU: AVR Microcontroller (like an ATmega328P on an Arduino Uno).
  ### Sensors: 1x Water Flow Sensor, 1x Push Button, Multiple Analog Soil Moisture Sensors.
  ### Actuator: 1x Water Pump with a driver (my implementation uses a relay).

## **Pinout Configuration**

  ### *Pins are defined in hal.h*

  #### Function: Start Trigger
    Microcontroller Pin: PB0 (PCINT0)
    Arduino Uno Pin: D8
    Notes: Wakes system from IDLE.

  #### Function: Flow Sensor
    Microcontroller Pin: PD2 (INT0)
    Arduino Uno Pin: D2
    Notes: External interrupt on falling edge. (Probably wired to a push button)

  #### Function: Pump Control
    Microcontroller Pin: PD3
    Arduino Uno Pin: D3
    Notes: Physical orientation is designed to stop water flow when pump is not running (without a valve).

  #### Function: Moisture Sensor #1
    Microcontroller Pin: PC0 (ADC0)
    Arduino Uno Pin: A0
    Notes: Analog input.

  #### Function: Moisture Sensor #2
    Microcontroller Pin: PC1 (ADC1)
    Arduino Uno Pin: A1
    Notes: Analog input.

## **Configuration**

  ### *Adjust parameters in plant_config.h*

    /* Adjust these constants for your physical setup */
    #define PLANTS_IN_GARDEN 2            // Adjust depending on # of plants currently in garden
    #define FLOW_RATE 562                 // Volumetric flow rate of pump in L/ms
    #define FLOW_CHECK_SETPOINT (FLOW_RATE * 2)   // Minimum time (ms) between pulses for Flow Check to pass
    #define FLOW_TIMEOUT_MS 5000          // 5000ms or 5 second timeout

## **Operation**

  ### Connect the hardware.
  ### Build and flash the code to the device.
  ### The system will start in an IDLE state.
  ### Pressing the start button on pin D8 begins the watering cycle.
  ### The system returns to IDLE when finished.
