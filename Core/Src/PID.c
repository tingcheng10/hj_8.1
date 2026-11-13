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
              float imaxout1,
              float imaxout2,
              uint8_t mode)
{
  if (pid != NULL)
  {
    memset(pid, 0, sizeof(PID_t));
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->maxout = fabsf(maxout);
    pid->imaxout1 = fabsf(imaxout1);
    pid->imaxout2 = fabsf(imaxout2);
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
  pid->pout = pid->error * pid->kp;

  if (pid->mode == LIMITING)
  {
    pid->iout += pid->ki * pid->error;
  }
  else
  {
    if (fabsf(pid->error) <= pid->imaxout2)
    {
      pid->iout = pid->iout + pid->ki * pid->error;
    }
  }

  if (pid->iout > pid->imaxout1)
  {
    pid->iout = pid->imaxout1;
  }
  if (pid->iout < -pid->imaxout1)
  {
    pid->iout = -pid->imaxout1;
  }

  pid->dout = pid->kd * (pid->error - pid->last_error);
  pid->output = pid->iout + pid->pout + pid->dout;
  if (pid->output > pid->maxout)
  {
    pid->output = pid->maxout;
  }
  if (pid->output < -pid->maxout)
  {
    pid->output = -pid->maxout;
  }
  pid->last_error = pid->error;
}

void PID_Reset(PID_t *pid)
{
  if (pid == NULL)
    return;
  pid->error = 0.0f;
  pid->last_error = 0.0f;
  pid->pout = 0.0f;
  pid->iout = 0.0f;
  pid->dout = 0.0f;
  pid->output = 0.0f;
}

