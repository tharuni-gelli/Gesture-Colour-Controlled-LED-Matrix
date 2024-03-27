/*
 * led.c
 *
 *  Created on: Dec 10, 2023
 *      Author: Shruthi Thallapally
 *
 * Description:
 * This file contains functions for controlling LEDs on a custom LED matrix setup.
 * It includes initialization of GPIOs, various LED display patterns, and helper functions
 * for setting and clearing individual LEDs based on color.
 */

#include "stm32f4xx.h"
#include "led.h"

volatile uint32_t msTicks;  // Variable to store elapsed milliseconds

/**
 * Initializes GPIO for LED control.
 * Sets up GPIO ports A, C, D, and E for output to control the LED matrix.
 */
void initGPIO() {
    // Enable GPIO clock for Ports A, C, D, and E
    RCC_AHB1ENR |= (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4);

    // Set GPIOA, GPIOC, GPIOD, GPIOE pins as output
    GPIOA_MODER &= ~(0xFFFFFFFF);
    GPIOA_MODER |= 0x55555555; // GPIOA_MODER
    GPIOC_MODER &= ~(0xFFFFFFFF);
    GPIOC_MODER |= 0x55555555; // GPIOC_MODER
    GPIOD_MODER &= ~(0xFFFFFFFF);
    GPIOD_MODER |= 0x55555555; // GPIOD_MODER
    GPIOE_MODER &= ~(0xFFFFFFFF);
    GPIOE_MODER |= 0x55555555; // GPIOE_MODER
}


/**
 * Displays an upward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayUpPattern(PredominantColor color) {
    for (int layer = 0; layer < 8; layer++) {
        // Turn on all LEDs in the current layer
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                setLED(layer, row, col,color);
            }
        }
        Delay_ms(1000);
        // Turn off all LEDs in the current layer
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                clearLED(layer, row, col,color);
            }
        }
        Delay_ms(1000);
    }
}

/**
 * Displays a downward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayDownPattern(PredominantColor color) {
    for (int layer = 7; layer >= 0; layer--) {
        // Turn on all LEDs in the current layer
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                setLED(layer, row, col,color);
            }
        }
        Delay_ms(1000);

        // Turn off all LEDs in the current layer
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                clearLED(layer, row, col,color);
            }
        }
        Delay_ms(1000);
    }
}

/**
 * Displays a rightward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayRightPattern(PredominantColor color) {
    for (int layer = 0; layer < 8; layer++) {
        for (int col = 0; col < 8; col++) {
            // Turn on a vertical line in the current column
            for (int row = 0; row < 8; row++) {
                setLED(layer, row, col,color);
            }
            Delay_ms(1000);
            // Turn off the vertical line in the current column
            for (int row = 0; row < 8; row++) {
                clearLED(layer, row, col,color);
            }
            Delay_ms(1000);
        }
    }
}

/**
 * Displays a leftward moving pattern on the LED matrix.
 *
 * Parameters:
 * color - The color in which the pattern is displayed.
 */
void displayLeftPattern(PredominantColor color) {
    for (int layer = 0; layer < 8; layer++) {
        for (int col = 7; col >= 0; col--) {
            // Turn on a vertical line in the current column
            for (int row = 0; row < 8; row++) {
                setLED(layer, row, col,color);
            }
            Delay_ms(1000);
            // Turn off the vertical line in the current column
            for (int row = 0; row < 8; row++) {
                clearLED(layer, row, col,color);
            }
            Delay_ms(1000);
        }
    }
}

/**
 * Sets an individual LED to a specified color.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 * color - The color to set the LED.
 */
void setLED(int layer, int row, int col,PredominantColor color) {
	LEDPin redPin = getRedPin(layer, row, col);
	    LEDPin greenPin = getGreenPin(layer, row, col);
	    LEDPin bluePin = getBluePin(layer, row, col);

	    // Set or clear the red component
	    if (color == RED) {
	        redPin.port->BSRR = (1U << redPin.pin); // Set red pin
	    } else {
	        redPin.port->BSRR = (1U << (redPin.pin + 16)); // Reset red pin
	    }

	    // Set or clear the green component
	    if (color==GREEN) {
	        greenPin.port->BSRR = (1U << greenPin.pin); // Set green pin
	    } else {
	        greenPin.port->BSRR = (1U << (greenPin.pin + 16)); // Reset green pin
	    }

	    // Set or clear the blue component
	    if (color==BLUE) {
	        bluePin.port->BSRR = (1U << bluePin.pin); // Set blue pin
	    } else {
	        bluePin.port->BSRR = (1U << (bluePin.pin + 16)); // Reset blue pin
	    }
}

/**
 * Clears an individual LED, turning it off.
 *
 * Parameters:
 * layer - The layer of the LED in the matrix.
 * row   - The row of the LED in the matrix.
 * col   - The column of the LED in the matrix.
 * color - The color to clear from the LED.
 */
void clearLED(int layer, int row, int col, PredominantColor color) {
    // Get the GPIO pins for the red, green, and blue components
    LEDPin redPin = getRedPin(layer, row, col);
    LEDPin greenPin = getGreenPin(layer, row, col);
    LEDPin bluePin = getBluePin(layer, row, col);

    // Reset the color components
    redPin.port->BSRR = (1U << (redPin.pin + 16)); // Reset red pin
    greenPin.port->BSRR = (1U << (greenPin.pin + 16)); // Reset green pin
    bluePin.port->BSRR = (1U << (bluePin.pin + 16)); // Reset blue pin
}

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
LEDPin getRedPin(int layer, int row, int col) {
    LEDPin pin;
    //  GPIOA is used for red pins
    pin.port = GPIOC;

    // mapping: pin number is calculated based on layer, row, and column
    pin.pin = layer * 16 + row; // Simplified example
    return pin;
}

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
LEDPin getGreenPin(int layer, int row, int col) {
    LEDPin pin;
    // GPIOB is used for green pins
    pin.port = GPIOD;

    //  mapping
    pin.pin = layer * 16 + col; // Simplified example
    return pin;
}

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
LEDPin getBluePin(int layer, int row, int col) {
    LEDPin pin;
    //GPIOC is used for blue pins
    pin.port = GPIOE;


    pin.pin = (layer * 16 + row + col) % 16; // Simplified example
    return pin;
}

/**
 * Implements a delay in milliseconds.
 *
 * Parameters:
 * ms - The number of milliseconds to delay.
 */
void Delay_ms(uint32_t ms) {
    msTicks = ms;  // Set the global variable with the desired delay
    while (msTicks != 0) {
        // Wait until the SysTick_Handler decrements the counter to zero
    }
}
