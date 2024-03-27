/*
 * color.c
 *
 * Created on: Dec 6, 2023
 * Author: Shruthi Thallapally
 *
 * Description: This module is designed to interact with the TCS34725 color sensor.
 * It includes initialization and functions to read and interpret color data.
 */

#include "color.h"
#include "i2c.h"
#include "stm32f4xx.h" // Include appropriate header file
#include "stdio.h"
#include "string.h"
#include "uart.h"

uint16_t r = 0, g = 0, b = 0, c = 0; // Variables to hold color data (red, green, blue, clear)

/**
 * Initializes the TCS34725 color sensor.
 * Sets up the sensor for color detection including power, integration time, and gain control.
 */
void TCS34725_Init(void) {
    write_i2c(TCS34725_ADDRESS, (0x80|TCS34725_ENABLE), 0x03); // Power on and enable ADC
    write_i2c(TCS34725_ADDRESS, (0x80| TCS34725_ATIME), 0xEB);  // Set integration time
    write_i2c(TCS34725_ADDRESS, (0x80|TCS34725_CONTROL), 0x02); // Set gain control to 16x
}

/**
 * Reads color data from the TCS34725 sensor and identifies the predominant color.
 *
 * Returns:
 * PredominantColor - The identified predominant color or UNKNOWN if unable to determine.
 */
PredominantColor TCS34725_ReadColorAndCheck() {

    // Read color data from the sensor
    c = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_CDATAL));
    r = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_RDATAL));
    g = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_GDATAL));
    b = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_BDATAL));

    // Determine the predominant color
    if(c > 2000) {
        printf("\n\rUnknown color\n\r");
        return UNKNOWN;
    } else if (r > g && r > b) {
        printf("\n\rDetected color is red\n\r");
        return RED;
    } else if (g > r && g > b) {
        printf("\n\rDetected color is green\n\r");
        return GREEN;
    } else if (b > r && b > g) {
        printf("\n\rDetected color is blue\n\r");
        return BLUE;
    }
}

/**
 * Reads raw color data (red, green, blue, clear) from the TCS34725 sensor.
 *
 * Parameters:
 * r, g, b, c - Pointers to store the read color data.
 */
void TCS34725_ReadColor(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    *c = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_CDATAL));
    *r = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_RDATAL));
    *g = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_GDATAL));
    *b = read_i2c_word(TCS34725_ADDRESS, (0x80|TCS34725_BDATAL));

    // Optional: Print color data for debugging
    // printf("\n\rRed: %u, Green: %u, Blue: %u, Clear: %u\n\r", *r, *g, *b, *c);
}
