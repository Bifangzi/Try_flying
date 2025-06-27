/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    printf("Default Task Running...\n");
    uint16_t x_value = 0;
    uint16_t y_value = 0;
    uint16_t temp_value = 0;
    uint16_t sum0 = 0;
    uint16_t sum1 = 0;
    uint16_t filterbuffer0[60] = {0};
    uint16_t filterbuffer1[60] = {0};

    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc2);

    for(int count=0;count<60;count++)
    {
      filterbuffer0[count] = (uint16_t)HAL_ADC_GetValue(&hadc1);
      filterbuffer1[count] = (uint16_t)HAL_ADC_GetValue(&hadc2);  
    }
    for(int i=0;i<60;i++)
    {
      for (int j =0; j< 60-i ; j++)
      {
        if (filterbuffer0[i] > filterbuffer0[i+1])
        {
          temp_value = filterbuffer0[i];
          filterbuffer0[i] = filterbuffer0[i+1];
          filterbuffer0[i+1] = temp_value;
        }
        if (filterbuffer1[i] > filterbuffer1[i+1])
        {
          temp_value = filterbuffer1[i];
          filterbuffer1[i] = filterbuffer1[i+1];
          filterbuffer1[i+1] = temp_value;
        }
      }
    }
    for (int count=20;count<40 ;count++)
    {
      sum0+= filterbuffer0[count];
      sum1+= filterbuffer1[count];
    }
    x_value = (uint16_t)((float)sum0 / 20);
    y_value = (uint16_t)((float)sum1 / 20);

    // HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    // x_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
    // HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
    // y_value = (uint16_t)HAL_ADC_GetValue(&hadc2);

    printf("%d,%d\n", x_value, y_value);
    osDelay(10);
    
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); // Toggle an LED connected to GPIOB Pin 0
    osDelay(500);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

