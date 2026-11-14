/*
 * can_motor.h
 *
 *  Created on: 2026年11月5日
 *      Author: tingcheng
 */

#ifndef CAN_MOTOR_H_
#define CAN_MOTOR_H_
#include"main.h"
extern CAN_HandleTypeDef hcan1;

void Motor_RightHandle(void);

void Motor_ErrorHandle(void);

void CAN_Init(CAN_HandleTypeDef *hcan);

void CAN_CmdChassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);


#define CAN_CHASSIS_ALL_ID 0x1FF
#define CHASSIS_CAN hcan1
#define MOTOR_CAN_RX_ID (0x204+1)

void CAN_FilterInit(CAN_HandleTypeDef *hcan);

/**
 * raw是编码器原始刻度，不是单位，需要换算
 */
typedef struct {
  volatile uint16_t temperature_c; /* 电机温度，单位 ℃，来自反馈帧 [6] 字节 */
  volatile uint16_t angle_raw; /* 机械角度原始值，0~8191（4096 = 180°），来自 [0..1] */
  volatile int16_t speed_rpm; /* 实际转速，单位 rpm，正负表示方向，来自 [2..3] */
  volatile int16_t current_raw; /* 实际电流原始值，16384 来自 [4..5] */
  volatile uint32_t last_rx_time_ms; /* 上次成功收到反馈帧的时刻，单位 ms（取自 HAL_GetTick） */
  volatile uint32_t now_tick_ms; /* 当前时刻，单位 ms，由上层任务每周期刷新，用于判超时 */
} MotorParam_t;

extern MotorParam_t motor_parameter;


#endif /* CAN_MOTOR_H_ */
