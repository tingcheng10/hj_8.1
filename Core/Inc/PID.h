/*
 * PID.h
 *
 *  Created on: 2026年11月6日
 *      Author: tingcheng
 */

#ifndef SRC_PID_H_
#define SRC_PID_H_
#define LIMITING 1
#define SEPARATING 0

typedef struct
{
  float kp;
  float ki;
  float kd;
  float error; //误差
  float last_error; //上次误差
  float target; //目标
  float output; //输出电压
  float p_out;
  float i_out;
  float d_out;
  float out_max; //输出限幅
  float integral_max; //积分限幅
  float integral_separate_threshold; //积分分离值rpm
  float feedback; //实际转速
  uint8_t mode;
} PID_t;

extern PID_t angle_pid;
extern PID_t speed_pid;
extern volatile float speed_target;

void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float out_max,
              float integral_max,
              float integral_separate_threshold,
              uint8_t mode);

void PID_Calc(PID_t *pid,
              float target,
              float back);

void PID_Reset(PID_t *pid);


#endif /* SRC_PID_H_ */
