/*
 * PID.c
 *
 *  Created on: 2026年11月6日
 *      Author: tingcheng
 */


#include<math.h>
#include<string.h>
#include"main.h"
#include"PID.h"


PID_t angle_pid;
PID_t speed_pid;
volatile float speed_target = 0.0f;

void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float maxout,
              float integral_max,
              float integral_separate_threshold,
              uint8_t mode)
{
  if (pid != NULL)
  {
    memset(pid, 0, sizeof(PID_t));
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->out_max = fabsf(maxout);
    pid->integral_max = fabsf(integral_max);
    pid->integral_separate_threshold = fabsf(integral_separate_threshold);
    pid->mode = mode;
  }
}

void PID_Calc(PID_t *pid, float target, float back)
{
  if (pid == NULL)
    return;
  pid->target = target;
  pid->feedback = back;
  pid->error = pid->target - pid->feedback;
  pid->p_out = pid->error * pid->kp;

  if (pid->mode == LIMITING)
  {
    pid->i_out += pid->ki * pid->error;
  }
  else
  {
    if (fabsf(pid->error) <= pid->integral_separate_threshold)
    {
      pid->i_out = pid->i_out + pid->ki * pid->error;
    }
  }

  if (pid->i_out > pid->integral_max)
  {
    pid->i_out = pid->integral_max;
  }
  if (pid->i_out < -pid->integral_max)
  {
    pid->i_out = -pid->integral_max;
  }

  pid->d_out = pid->kd * (pid->error - pid->last_error);
  pid->output = pid->i_out + pid->p_out + pid->d_out;
  if (pid->output > pid->out_max)
  {
    pid->output = pid->out_max;
  }
  if (pid->output < -pid->out_max)
  {
    pid->output = -pid->out_max;
  }
  pid->last_error = pid->error;
}

void PID_Reset(PID_t *pid)
{
  if (pid == NULL)
    return;
  pid->error = 0.0f;
  pid->last_error = 0.0f;
  pid->p_out = 0.0f;
  pid->i_out = 0.0f;
  pid->d_out = 0.0f;
  pid->output = 0.0f;
}

