/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/**
 * @file main.c
 * @brief Main program for LED control with gesture recognition.
 * Created on: Dec 10, 2023
 * Author: Shruthi Thallapally
 *
 * This program initializes the necessary peripherals for LED control and gesture recognition.
 * It uses an APDS9960 sensor for gesture detection and a TCS34725 sensor for color recognition.
 * Based on the detected gesture, it displays different LED patterns on a custom LED matrix.
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"
#include "color.h"
#include "uart.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "gesture.h"
#include "i2c.h"


char rxData;

// Function Prototypes
void SystemClock_Config(void);
void SysTick_Handler(void);
void SysTick_Init(void);
void Delay_ms(uint32_t ms) ;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  gesture_t gesture;
  // Initialize peripherals
  i2c_gpio_init();
  i2c_init();
  uint8_t a=0;
Here:
  apds9960_init();
  a=check_gesture_init();
  if(a==0){
	  printf("Init failed for gesture sensor\n\r");
	  goto Here;
  }
  PredominantColor color;
  TCS34725_Init();
  SysTick_Init();
  USART2_Config();
  initGPIO();
  printf("\n\rIn main function\n\r");
  printf("\n\rWaiting for Color input\n\r");

  while (1)
  {
	color=TCS34725_ReadColorAndCheck();
	Delay_ms(1000);
// Process the color data
// printf("\n\rRed: %u, Green: %u, Blue: %u, Clear: %u\n\r", r, g, b, c);

	while(color!= UNKNOWN){
		printf("\n\r Waiting for gesture\n\r");
	  if (gesture_data_available()) {
	          gesture = detect_gesture();
	          Delay_ms(1000);

	          switch (gesture) {
	          		case GESTURE_UP:
	          			 // Handle UP gesture
	          		    printf("\n\r UP\n\r");
	          		    displayUpPattern(color);
	          		    break;
	          		case GESTURE_DOWN:
	          			// Handle DOWN gesture
	          		     printf("\n\r DOWN\n\r");
	          		     displayDownPattern(color);
	          		     break;
	          		case GESTURE_LEFT:
	          		  // Handle LEFT gesture
	          		     printf("\n\r LEFT\n\r");
	          		     displayLeftPattern(color);
	          		     break;
	          		case GESTURE_RIGHT:
	          			// Handle RIGHT gesture
	          		     printf("\n\r RIGHT\n\r");
	          		     displayRightPattern(color) ;
	          		     break;
	          		default:
	          		     printf("\n\r Not A valid gesture\n\r");
	          		                  // No valid gesture detected
	          		     break;
	          		}
	          printf("\n\r BLINKING LEDs\n\r");
	          Delay_ms(1000);
	          break;
	      }

	  Delay_ms(2000);
	}
  }

}

/**
 * @brief SysTick interrupt handler.
 * Decrements the millisecond counter.
 */
void SysTick_Handler(void) {
    if (msTicks != 0) {
        msTicks--;  // Decrement the milliseconds counter if not already zero
    }
}

/**
 * @brief Initializes SysTick with 1ms interval.
 * Sets up the SysTick timer for 1ms ticks.
 */
void SysTick_Init(void) {
    SystemCoreClockUpdate();  // Update system clock variable
    SysTick_Config(SystemCoreClock / 1000);  // Configure SysTick for 1ms intervals
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
