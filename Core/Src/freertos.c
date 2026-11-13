/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include <main.h>
#include "cmsis_os.h"
#include "can_motor.h"
#include "PID.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

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
volatile uint8_t motor_online = 0;
volatile int32_t servo_angle_deg  = 0;
volatile uint8_t key_pressed = 0;
volatile uint32_t key_last_tick_ms = 0;

/* USER CODE END Variables */
/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
  .name = "keyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
  .name = "motorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for uartrxTask */
osThreadId_t uartrxTaskHandle;
const osThreadAttr_t uartrxTask_attributes = {
  .name = "uartrxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uarttxTask */
osThreadId_t uarttxTaskHandle;
const osThreadAttr_t uarttxTask_attributes = {
  .name = "uarttxTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uartQueue */
osMessageQueueId_t uartQueueHandle;
const osMessageQueueAttr_t uartQueue_attributes = {
  .name = "uartQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartkeyTask(void *argument);
void StartmotorTask(void *argument);
void StartuartrxTask(void *argument);
void StartledTask(void *argument);
void StartuarttxTask(void *argument);

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

  /* Create the queue(s) */
  /* creation of uartQueue */
  uartQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &uartQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of keyTask */
  keyTaskHandle = osThreadNew(StartkeyTask, NULL, &keyTask_attributes);

  /* creation of motorTask */
  motorTaskHandle = osThreadNew(StartmotorTask, NULL, &motorTask_attributes);

  /* creation of uartrxTask */
  uartrxTaskHandle = osThreadNew(StartuartrxTask, NULL, &uartrxTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(StartledTask, NULL, &ledTask_attributes);

  /* creation of uarttxTask */
  uarttxTaskHandle = osThreadNew(StartuarttxTask, NULL, &uarttxTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartkeyTask */
/**
  * @brief  Function implementing the keyTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartkeyTask */
void StartkeyTask(void *argument)
{
  /* USER CODE BEGIN StartkeyTask */
  (void) argument;
  /* Infinite loop */
  /*舵机转动方向，1左，0右*/
  int servo_dir = 0;
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500u);
  for (;;)
  {
    if (key_pressed == 1 && (HAL_GetTick() - key_last_tick_ms >= 30))
    {
      if (servo_dir == 0)
      {
        servo_angle_deg += 20;
        if (servo_angle_deg >= 180)
        {
          servo_dir = 1;
          servo_angle_deg = 180;
        }
      }
      else
      {
        servo_angle_deg -= 20;
        if (servo_angle_deg <= 0)
        {
          servo_dir = 0;
          servo_angle_deg = 0;
        }
      }
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,
                            500u+(uint32_t)servo_angle_deg * 2000u / 180u);
      key_pressed = 0;
    }
    osDelay(10);
  }
  /* USER CODE END StartkeyTask */
}

/* USER CODE BEGIN Header_StartmotorTask */
/**
* @brief Function implementing the motorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartmotorTask */
void StartmotorTask(void *argument)
{
  /* USER CODE BEGIN StartmotorTask */
  (void) argument;
  /* Infinite loop */
  for (;;)
  {
    motor_parameter.now_tick_ms = HAL_GetTick();
    if (motor_parameter.now_tick_ms - motor_parameter.last_rx_time_ms >= 100)
    {
      motor_online = 0;
      PID_Reset(&speed_pid);
      CAN_CmdChassis(0, 0, 0, 0);
    }
    else
    {
      motor_online = 1;
      PID_Calc(&speed_pid, speed_target, (float) motor_parameter.speed_rpm);
      CAN_CmdChassis((int16_t) speed_pid.output, 0, 0, 0);
    }


    osDelay(1);
  }
  /* USER CODE END StartmotorTask */
}

/* USER CODE BEGIN Header_StartuartrxTask */
/**
* @brief Function implementing the uartrxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartuartrxTask */
void StartuartrxTask(void *argument)
{
  /* USER CODE BEGIN StartuartrxTask */
  (void) argument;
  /* Infinite loop */
  uint16_t ch;
  char line[32];
  uint8_t idx = 0;

  for (;;)
  {
    if (osMessageQueueGet(uartQueueHandle, &ch, NULL, osWaitForever) == osOK)
    {
      if (ch == '\n' || ch == '\r')
      {
        if (idx > 0)
        {
          line[idx] = '\0';
          if (strncmp(line, "motor", 5) == 0 && idx > 6)
          {
            if (line[5] == '+')
            {
              speed_target = (float) atoi(&line[6]);
            }
            if (line[5] == '-')
            {
              speed_target = -(float) atoi(&line[6]);
            }
            if (speed_target > 150.0f) speed_target = 150.0f;
            if (speed_target < -150.0f) speed_target = -150.0f;
          }
          idx = 0;
        }
      }
      else if (idx < sizeof(line) - 1)
      {
        line[idx] = (char) ch;
        idx++;
      }
    }
  }
  /* USER CODE END StartuartrxTask */
}

/* USER CODE BEGIN Header_StartledTask */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartledTask */
void StartledTask(void *argument)
{
  /* USER CODE BEGIN StartledTask */
  (void) argument;
  int32_t duty = 0;
  int32_t step = 20;
  int8_t flag2 = 0;
  /* Infinite loop */
  for (;;)
  {
    if (motor_online == 1)
    {
      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 1000);
      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
    } else
    {
      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0);

      if (flag2 == 0)
      {
        duty += step;
        if (duty >= 1000)
        {
          duty = 1000;
          flag2 = 1;
        }
      }
      else
      {
        duty -= step;
        if (duty <= 0)
        {
          duty = 0;
          flag2 = 0;
        }
      }

      __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, (uint32_t)duty);
    }

    osDelay(20);
  }
  /* USER CODE END StartledTask */
}

/* USER CODE BEGIN Header_StartuarttxTask */
/**
* @brief Function implementing the uarttxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartuarttxTask */
void StartuarttxTask(void *argument)
{
  /* USER CODE BEGIN StartuarttxTask */
  (void) argument;
  /* Infinite loop */
  char buf[64];
  int len;

  for (;;)
  {
    len = snprintf(buf, sizeof(buf), "spd=%d\r\n",
                   (int) speed_pid.feedback);

    HAL_UART_Transmit(&huart6, (uint8_t *) buf, (uint16_t) len, 100);

    osDelay(2000);
  }
  /* USER CODE END StartuarttxTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == KEY_Pin)
  {
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_RESET)
    {
      key_last_tick_ms = HAL_GetTick();
      key_pressed = 1;
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART6)
  {
    uint16_t v = (uint16_t) rx_byte;
    osMessageQueuePut(uartQueueHandle, &v, 0, 0);
    HAL_UART_Receive_IT(&huart6, &rx_byte, 1);
  }
}

/* USER CODE END Application */

