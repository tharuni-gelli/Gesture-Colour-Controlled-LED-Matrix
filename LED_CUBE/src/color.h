/*
 * color.h
 *
 * Created on: Dec 6, 2023
 * Author: Shruthi Thallapally
 *
 * Description: Header file for color sensor module.
 * This file contains definitions and function prototypes for interfacing with the TCS34725 color sensor.
 */

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include "stdint.h"

// I2C address for the TCS34725 color sensor
#define TCS34725_ADDRESS 0x29

// TCS34725 register addresses
#define TCS34725_ENABLE  0x00 // Enable register
#define TCS34725_ATIME   0x01 // Integration time register
#define TCS34725_CONTROL 0x0F // Control register (gain settings)
#define TCS34725_CDATAL  0x14 // Lower byte of clear channel data
#define TCS34725_RDATAL  0x16 // Lower byte of red channel data
#define TCS34725_GDATAL  0x18 // Lower byte of green channel data
#define TCS34725_BDATAL  0x1A // Lower byte of blue channel data

// Global variables for color data
extern uint16_t r, g, b, c; // Red, Green, Blue, and Clear color values

/**
 * Enumeration for predominant colors.
 */
typedef enum {
    RED,     // Predominant red color
    GREEN,   // Predominant green color
    BLUE,    // Predominant blue color
    UNKNOWN  // Color is unknown or not identifiable
} PredominantColor;

// Function prototypes

/**
 * Initializes the TCS34725 color sensor.
 */
void TCS34725_Init(void);

/**
 * Reads and identifies the predominant color from the TCS34725 sensor.
 *
 * Returns:
 * PredominantColor - the predominant color detected.
 */
PredominantColor TCS34725_ReadColorAndCheck();

/**
 * Reads raw color data (red, green, blue, clear) from the TCS34725 sensor.
 *
 * Parameters:
 * r, g, b, c - Pointers to store the read color data.
 */
void TCS34725_ReadColor(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);

// Optional SysTick_Handler function declaration (commented out as it may not be needed in this context)
// void SysTick_Handler(void);

#endif /* SRC_COLOR_H_ */
