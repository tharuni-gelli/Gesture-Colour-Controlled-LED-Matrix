/*
 * gesture.h
 *
 * Created on: Dec 6, 2023
 * Author: Shruthi Thallapally
 *
 * Description: Header file for the gesture detection module.
 * This file contains definitions, constants, and function prototypes
 * for interfacing with the APDS9960 gesture sensor.
 */

#ifndef SRC_GESTURE_H_
#define SRC_GESTURE_H_

#include "stdint.h"

// I2C and GPIO constants for APDS9960
#define APDS9960_I2C_ADDRESS  0x39 // I2C address of APDS9960
#define APDS9960_GSTATUS      0xAF // Address of the GSTATUS register
#define APDS9960_ID           0x92 // Device ID register address

// GPIO pin definitions (modify as per your hardware configuration)
#define I2C1_SCL_PIN          GPIO_PIN_6
#define I2C1_SDA_PIN          GPIO_PIN_7
#define I2C1_GPIO_PORT        GPIOB

// APDS9960 register addresses
#define APDS9960_ENABLE       0x80
#define APDS9960_ATIME        0x81
#define APDS9960_CONTROL      0x8F
#define APDS9960_GCONF4       0xAB
#define APDS9960_GCONF3       0xAA
#define APDS9960_GCONF1       0xA2
#define APDS9960_GCONF2       0xA3
#define APDS9960_GPENTH       0xA0
#define APDS9960_GEXTH        0xA1
#define APDS9960_GPULSE       0xA6
#define APDS9960_GOFFSET_U    0xA4
#define APDS9960_GOFFSET_D    0xA5
#define APDS9960_GOFFSET_L    0xA7
#define APDS9960_GOFFSET_R    0xA9
#define APDS9960_GFIFO_U      0xFC
#define APDS9960_GFIFO_D      0xFD
#define APDS9960_GFIFO_L      0xFE
#define APDS9960_GFIFO_R      0xFF

#define GESTURE_THRESHOLD 30 // Threshold for gesture detection

// Enumeration for possible gesture types
typedef enum {
    GESTURE_NONE,   // No gesture detected
    GESTURE_UP,     // Upward swipe gesture
    GESTURE_DOWN,   // Downward swipe gesture
    GESTURE_LEFT,   // Left swipe gesture
    GESTURE_RIGHT   // Right swipe gesture
} gesture_t;

/* Function Prototypes */

/**
 * Initializes the APDS9960 gesture sensor.
 */
void apds9960_init();

/**
 * Resets the internal gesture detection counts.
 */
void resetCounts();

/**
 * Checks if the gesture sensor is properly initialized.
 *
 * Returns:
 * 1 if initialization is successful, 0 otherwise.
 */
int check_gesture_init();

/**
 * Detects and returns the type of gesture detected by the APDS9960 sensor.
 *
 * Returns:
 * gesture_t - the type of gesture detected.
 */
gesture_t detect_gesture();

/**
 * Checks if gesture data is available from the sensor.
 *
 * Returns:
 * 1 if data is available, 0 otherwise.
 */
uint8_t gesture_data_available();

#endif /* SRC_GESTURE_H_ */
