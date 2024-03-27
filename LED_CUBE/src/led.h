/*
 * led.h
 *
 *  Created on: Dec 10, 2023
 *      Author: Shruthi Thallapally
 *
 * Description:
 * This header file defines the function prototypes and structures for controlling a custom LED matrix.
 * It includes functions for initializing GPIOs for LED control, displaying patterns based on the
 * predominant color detected, and manipulating individual LEDs in the matrix.

 */

#ifndef SRC_LED_H_
#define SRC_LED_H_
#include "color.h"
#include "gesture.h"
#include "stdint.h"

// Register addresses for GPIO port modes and output data registers
#define RCC_AHB1ENR   (*((volatile uint32_t*)0x40023830))
#define GPIOA_MODER   (*((volatile uint32_t*)0x40020000))
#define GPIOC_MODER   (*((volatile uint32_t*)0x40020800))
#define GPIOD_MODER   (*((volatile uint32_t*)0x40020C00))
#define GPIOE_MODER   *((volatile uint32_t*)(0x40021000))
#define GPIOA_ODR (*((volatile uint32_t*)0x40020414))
#define GPIOC_ODR (*((volatile uint32_t*)0x40020814))
#define GPIOD_ODR (*((volatile uint32_t*)0x40020C14))
#define GPIOE_ODR (*((volatile uint32_t*)0x40021014))


// Structure for LED pin configuration
typedef struct {
    GPIO_TypeDef* port; // GPIO port
    uint16_t pin;       // GPIO pin number
} LEDPin;

extern volatile uint32_t msTicks;  // Variable to store elapsed milliseconds

/**
 * Initializes GPIO for LED control.
 * Sets up GPIO ports for output to control the LED matrix.
 */
void initGPIO();

/**
 * Displays an upward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayUpPattern(PredominantColor color);

/**
 * Displays a downward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayDownPattern(PredominantColor color);

/**
 * Displays a rightward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayRightPattern(PredominantColor color);

/**
 * Displays a leftward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayLeftPattern(PredominantColor color);

/**
 * Sets an individual LED to a specified color.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 * color - The color to set the LED.
 */
void setLED(int layer, int row, int col, PredominantColor color);

/**
 * Clears an individual LED, turning it off.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 * color - The color to clear from the LED.
 */
void clearLED(int layer, int row, int col, PredominantColor color);

/**
 * Gets the red component pin for an individual LED.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 *
 * Returns:
 * LEDPin - The pin controlling the red component of the LED.
 */
LEDPin getRedPin(int layer, int row, int col);

/**
 * Gets the green component pin for an individual LED.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 *
 * Returns:
 * LEDPin - The pin controlling the green component of the LED.
 */
LEDPin getGreenPin(int layer, int row, int col);

/**
 * Gets the blue component pin for an individual LED.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 *
 * Returns:
 * LEDPin - The pin controlling the blue component of the LED.
 */
LEDPin getBluePin(int layer, int row, int col);

/**
 * Implements a delay in milliseconds.
 *
 * Parameters:
 * ms - The number of milliseconds to delay.
 */
void Delay_ms(uint32_t ms);

#endif /* SRC_LED_H_ */
