/*
 * uart.h
 *
 *  Created on: Dec 8, 2023
 *      Author: Shruthi thallapally
 *
 * Description:
 * This header file defines the prototypes for UART communication functions using USART2
 * on STM32F4xx microcontrollers. It includes functions for configuring UART, transmitting
 * a single character, and receiving a single character. Additionally, it provides a
 * function for redirecting standard output to UART for use with printf().
 *
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

/**
 * Configures USART2 for UART communication.
 * This function sets up the necessary registers and configurations for UART communication
 * using USART2, including baud rate, mode, and enabling necessary peripherals.
 */
void USART2_Config(void);

/**
 * Transmits a single character over UART2.
 * This function sends a character via USART2, waiting until the transmission buffer is empty.
 *
 * Parameters:
 * ch - The character to be transmitted.
 */
void UART2_TxChar(char ch);

/**
 * Receives a single character over UART2.
 * This function waits for and returns a character received via USART2.
 *
 * Returns:
 * char - The received character.
 */
char UART2_RxChar();

/**
 * Overrides the standard _write function for redirecting printf() output to UART.
 * This function sends a string of characters via USART2, allowing printf() to output
 * to the UART terminal.
 *
 * Parameters:
 * file - File descriptor (not used).
 * ptr  - Pointer to the character array to be transmitted.
 * len  - Number of characters to transmit.
 *
 * Returns:
 * int - The number of characters transmitted.
 */
int _write(int file, char *ptr, int len);

#endif /* SRC_UART_H_ */
