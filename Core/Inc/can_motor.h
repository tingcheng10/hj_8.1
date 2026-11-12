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

void RIGHTHANDLE(void);
void ERRORHANDLE(void);
void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);



#define CAN_CHASSIS_ALL_ID 0x1FF
#define CHASSIS_CAN hcan1
#define MOTOR_CAN_RX_ID (0x204+1)

void CAN_FILTER_Init(CAN_HandleTypeDef *hcan);

typedef struct {
	volatile uint16_t temp;
	volatile uint16_t angle;
	volatile int16_t speed;
	volatile int16_t current;
	volatile uint32_t last_time;
	volatile uint32_t now;
}motor_par;
extern motor_par motor_parameter;


#endif /* CAN_MOTOR_H_ */
