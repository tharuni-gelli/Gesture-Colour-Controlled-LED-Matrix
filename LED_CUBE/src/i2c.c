/*
 * i2c.c
 *
 *  Created on: Dec 6, 2023
 *      Author: Shruthi Thallapally
 */
#include "i2c.h"
#include "stm32f4xx.h" // Include appropriate header file
#include "gesture.h"
#include "stdint.h"
#include "color.h"


/**
 * Initializes GPIO pins for I2C communication.
 * This function sets GPIOB pins 6 and 7 to alternate function mode for I2C communication.
 */
void i2c_gpio_init() {
    // Enable GPIOB clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Set PB6 and PB7 to alternate function, open-drain, pull-up
    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 );
    GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 );
    GPIOB->OTYPER |= (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 );
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 );
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0 );
    GPIOB->AFR[0] |= (0x04 << (4 * 6)) | (0x04 << (4 * 7)); // AF4 for I2C1
     //   GPIOB->AFR[1] |= (0x04 << ((8 - 8) * 4)) | (0x04 << ((9 - 8) * 4)); // AF4 for I2C1 (PB8 and PB9)

}

/**
 * Initializes the I2C1 peripheral.
 * This function sets up I2C1 for standard mode with appropriate clock and timing settings.
 */
void i2c_init() {
    // Enable I2C1 clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Reset I2C1
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    // Configure I2C1
    I2C1->CR1 &= ~I2C_CR1_PE; // Disable I2C1
    I2C1->CR2 = 16; // Set APB1 clock frequency
    I2C1->CCR = 80; // Set CCR for standard mode
    I2C1->TRISE = 17; // Set TRISE
    I2C1->CR1 |= I2C_CR1_PE; // Enable I2C1
}

/**
 * Generates an I2C start condition.
 * This function sends an I2C start signal to begin a transmission.
 */
void i2c_start() {
    I2C1->CR1 |= I2C_CR1_START; // Generate start condition
    while (!(I2C1->SR1 & I2C_SR1_SB)); // Wait for start condition to be generated
}

/**
 * Generates an I2C stop condition.
 * This function sends an I2C stop signal to end a transmission.
 */
void i2c_stop() {
    I2C1->CR1 |= I2C_CR1_STOP; // Generate stop condition
}

/**
 * Reads a byte from the I2C bus with acknowledgment.
 * This function reads a single byte and acknowledges the receipt.
 *
 * Returns:
 * uint8_t - The byte read from the I2C bus.
 */
uint8_t i2c_read_ack() {
    I2C1->CR1 |= I2C_CR1_ACK; // Enable ACK
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Wait until data register is not empty
    return I2C1->DR;
}

/**
 * Reads a byte from the I2C bus without acknowledgment.
 * This function reads a single byte without acknowledging the receipt.
 *
 * Returns:
 * uint8_t - The byte read from the I2C bus.
 */
uint8_t i2c_read_nack() {
    I2C1->CR1 &= ~I2C_CR1_ACK; // Disable ACK
    i2c_stop(); // Send stop condition
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Wait until data register is not empty
    return I2C1->DR;
}

/**
 * Writes a byte to a specific register of a specified I2C device.
 *
 * Parameters:
 * deviceAddr - The I2C address of the device.
 * reg        - The register address to write to.
 * data       - The data byte to be written.
 */
void write_i2c(uint8_t deviceAddr, uint8_t reg, uint8_t data) {
    // Start I2C transmission
    i2c_start();

    // Send device address with write operation
    I2C1->DR = (deviceAddr << 1) | 0;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Send register address
    I2C1->DR = reg;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    // Send data
    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    // Stop I2C transmission
    i2c_stop();
}

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
uint8_t read_i2c(uint8_t deviceAddr, uint8_t reg) {
    uint8_t data;

    // Start I2C transmission
    i2c_start();

    // Send device address with write operation
    I2C1->DR = (deviceAddr << 1) | 0;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Send register address
    I2C1->DR = reg;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    // Repeated start for read operation
    i2c_start();

    // Send device address with read operation
    I2C1->DR = (deviceAddr << 1) | 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Prepare for reception
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;

    // Receive data
    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data = I2C1->DR;

    i2c_stop();

    return data;
}

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
uint16_t read_i2c_word(uint8_t deviceAddr, uint8_t reg) {
    uint16_t data = 0;
    uint8_t data1, data2;

    // Start I2C transmission
    i2c_start();

    // Send device address with write operation
    I2C1->DR = (deviceAddr << 1) | 0;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Send register address
    I2C1->DR = reg;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    // Repeated start for read operation
    i2c_start();

    // Send device address with read operation
    I2C1->DR = (deviceAddr << 1) | 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Enable ACK for reception of multiple bytes
    I2C1->CR1 |= I2C_CR1_ACK;

    // Read first byte (MSB)
    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data1 = I2C1->DR;

    // Disable ACK and set STOP bit to prepare for last byte reception
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;

    // Read second byte (LSB)
    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data2 = I2C1->DR;

    // Combine the two bytes
    data = ((uint16_t)data2 << 8) |(uint16_t) (data1 & 0xff);

    return data;
}
