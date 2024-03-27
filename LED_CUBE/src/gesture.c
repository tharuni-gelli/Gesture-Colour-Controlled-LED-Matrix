/*
 * gesture.c
 *
 *  Created on: Dec 6, 2023
 *      Author: Shruthi Thallapally
 *
 * Description: This module is designed to interface with the APDS9960 sensor for gesture detection.
 *
 */
#include "stdint.h"
#include "math.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "gesture.h"
#include "i2c.h"



uint8_t gestCnt;

  uint8_t UCount;
  uint8_t DCount;

  uint8_t LCount;
  uint8_t RCount;

 /**
 * Initializes the APDS9960 gesture sensor.
 * Configures the necessary registers and settings for gesture detection.
 */
void apds9960_init()
{
	// Enable the sensor and set up gesture detection parameters
	write_i2c(APDS9960_I2C_ADDRESS, 0x80, 0x01); // ENABLE register: Power ON

	// Configure gesture sensor settings
	write_i2c(APDS9960_I2C_ADDRESS, 0xAA, 0x00); // GCONF3: Both pairs active
	write_i2c(APDS9960_I2C_ADDRESS, 0xA2, 0x00); // GFIFOTHreshold
	write_i2c(APDS9960_I2C_ADDRESS, 0xA3, 0x57); // GCONF2: Gesture Gain Control, LED Drive Strength, Gesture Wait Time
	write_i2c(APDS9960_I2C_ADDRESS, 0xA6, 0x80); // GPULSE: Gesture Pulse Count and Length
	write_i2c(APDS9960_I2C_ADDRESS, 0xAB, 0x01); // GCONF4: GMODE set to 1

	// Set gesture proximity and offset parameters
	write_i2c(APDS9960_I2C_ADDRESS, 0xA0, 0x50); // GPENTH: Gesture Proximity Entry Threshold
	write_i2c(APDS9960_I2C_ADDRESS, 0xA1, 0x1F); // GEXTH: Gesture Exit Threshold
	write_i2c(APDS9960_I2C_ADDRESS, 0xA4, 0x00); // GOFFSET_U: Gesture Offset UP
	write_i2c(APDS9960_I2C_ADDRESS, 0xA5, 0x00); // GOFFSET_D: Gesture Offset DOWN
	write_i2c(APDS9960_I2C_ADDRESS, 0xA7, 0x00); // GOFFSET_L: Gesture Offset LEFT
	write_i2c(APDS9960_I2C_ADDRESS, 0xA9, 0x00); // GOFFSET_R: Gesture Offset RIGHT

	// Finalize configuration
	write_i2c(APDS9960_I2C_ADDRESS, 0x80, 0x45); // ENABLE register: Gesture Enable + Power ON

	// Reset gesture detection counters
	resetCounts();
}
/**
 * Checks if the gesture sensor is properly initialized.
 *
 * Returns:
 * 1 if initialization is successful, 0 otherwise.
 */
int check_gesture_init() {
    uint8_t x = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_ID);
    return (x == 0xAB) ? 1 : 0;
}


/**
 * Detects and returns the type of gesture detected by the APDS9960 sensor.
 *
 * Returns:
 * gesture_t - the type of gesture detected (e.g., GESTURE_UP, GESTURE_DOWN, etc.).
 */
gesture_t detect_gesture() {
    uint8_t fifo_level, u_data, d_data, l_data, r_data, up_down_diff, left_right_diff;
    gesture_t gesture = GESTURE_NONE;

    // Read gesture FIFO level and data, then calculate differences
    fifo_level = read_i2c(APDS9960_I2C_ADDRESS, 0xAE);
    for (int i = 0; i < fifo_level; i++) {
        u_data = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_GFIFO_U);
        d_data = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_GFIFO_D);
        l_data = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_GFIFO_L);
        r_data = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_GFIFO_R);

        up_down_diff += u_data - d_data;
        left_right_diff += l_data - r_data;
    }

    // Determine gesture based on difference values
    if (abs(left_right_diff) > abs(up_down_diff)) {
        if (left_right_diff > GESTURE_THRESHOLD) {
            gesture = GESTURE_RIGHT;
        } else if (left_right_diff < -GESTURE_THRESHOLD) {
            gesture = GESTURE_LEFT;
        }
    } else {
        if (up_down_diff > GESTURE_THRESHOLD) {
            gesture = GESTURE_UP;
        } else if (up_down_diff < -GESTURE_THRESHOLD) {
            gesture = GESTURE_DOWN;
        }
    }

    return gesture; // Return the detected gesture (or GESTURE_NONE if no gesture detected)
}

/**
 * Resets the gesture detection counters.
 */
void resetCounts() {
    gestCnt = 0;
    UCount = 0;
    DCount = 0;
    LCount = 0;
    RCount = 0;
}
/**
 * Checks if gesture data is available from the sensor.
 *
 * Returns:
 * 1 if data is available, 0 otherwise.
 */
uint8_t gesture_data_available() {
    uint8_t gstatus = read_i2c(APDS9960_I2C_ADDRESS, APDS9960_GSTATUS);
    return (gstatus & 0b00000001) ? 1 : 0;
}

