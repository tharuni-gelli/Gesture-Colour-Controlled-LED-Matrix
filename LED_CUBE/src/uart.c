/*
 * uart.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Shruthi Thallapally
 *
 *
 * Description:
 * This file contains the implementation of UART communication functions for STM32F4xx microcontrollers.
 * It includes configuration of UART2 for basic transmit and receive capabilities.
 *
 */

#include "stm32f4xx.h"
#include "uart.h"
#include "main.h"
#include "color.h"
#include "uart.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "i2c.h"

/**
 * Configures USART2 for UART communication.
 * Sets up GPIO pins for USART2, configures baud rate, and enables the transmitter and receiver.
 */
void USART2_Config(void) {
    // Enable USART2 and GPIOA clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure GPIO pins for USART2 (PA2 = Tx, PA3 = Rx)
    GPIOA->AFR[0] = (7 << (4 * 2)) | (7 << (4 * 3));
    GPIOA->MODER = (GPIOA->MODER & ~(3 << (2 * 2))) | (2 << (2 * 2)) | (2 << (2 * 3));

    // Configure USART2
    USART2->BRR = 0x683; // 9600 baud rate @ 16MHz
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE  | USART_CR1_UE; // Enable transmitter and USART


}

/**
  * @brief Transmit a character via UART2
  * @param ch: Character to be transmitted
  */
void UART2_TxChar(char ch) {
    while (!(USART2->SR & USART_SR_TXE))
    	{
    	;
    	}

    USART2->DR = ch;
}

/**
  * @brief Receive a character via UART2
  * @return Received character
  */
char UART2_RxChar() {
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}

/**
 * Overrides the _write function to redirect printf() to UART2.
 *
 * Parameters:
 * file - File descriptor.
 * ptr  - Pointer to the buffer containing characters to transmit.
 * len  - Number of characters to transmit.
 *
 * Returns:
 * int - The number of characters transmitted.
 */
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        while (!(USART2->SR & USART_SR_TXE));  // Wait until TX buffer is empty
        USART2->DR = (ptr[i] & 0xFF);  // Send a character
    }
    return len;
}
