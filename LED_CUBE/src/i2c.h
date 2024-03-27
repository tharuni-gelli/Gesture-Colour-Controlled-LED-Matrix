/*
 * i2c.h
 *
 * Created on: Dec 6, 2023
 * Author: Shruthi Thallapally
 *
 * Description:
 * This header file defines the prototypes for I2C communication functions for STM32F4xx microcontrollers.
 * It includes functions for initializing I2C peripheral and GPIO pins, and for starting and stopping
 * I2C communication. Additionally, it provides functions for reading and writing data over the I2C bus,
 * which can be used for interfacing with various I2C devices.
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "stdint.h"

/**
 * Initializes GPIO pins for I2C communication.
 */
void i2c_gpio_init();

/**
 * Initializes the I2C1 peripheral for communication.
 */
void i2c_init();

/**
 * Generates an I2C start condition.
 */
void i2c_start();

/**
 * Generates an I2C stop condition.
 */
void i2c_stop();

/**
 * Reads a byte from the I2C bus with acknowledgment.
 *
 * Returns:
 * uint8_t - The byte read from the I2C bus.
 */
uint8_t i2c_read_ack();

/**
 * Reads a byte from the I2C bus without acknowledgment.
 *
 * Returns:
 * uint8_t - The byte read from the I2C bus.
 */
uint8_t i2c_read_nack();

/**
 * Writes a byte to a specific register of a specified I2C device.
 *
 * Parameters:
 * deviceAddr - The I2C address of the device.
 * reg        - The register address to write to.
 * data       - The data byte to be written.
 */
void write_i2c(uint8_t deviceAddr, uint8_t reg, uint8_t data);

/**
 * Reads a byte from a specific register of a specified I2C device.
 *
 * Parameters:
 * deviceAddr - The I2C address of the device.
 * reg        - The register address to read from.
 *
 * Returns:
 * uint8_t - The byte read from the specified register.
 */
uint8_t read_i2c(uint8_t deviceAddr, uint8_t reg);

/**
 * Reads a 16-bit word from a specific register of a specified I2C device.
 *
 * Parameters:
 * deviceAddr - The I2C address of the device.
 * reg        - The register address to read from.
 *
 * Returns:
 * uint16_t - The 16-bit word read from the specified register.
 */
uint16_t read_i2c_word(uint8_t deviceAddr, uint8_t reg);

#endif /* SRC_I2C_H_ */
