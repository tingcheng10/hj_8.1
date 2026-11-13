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
  pid->back = back;
  pid->e = pid->target - pid->back;
  pid->pout = pid->e * pid->kp;

  if (pid->mode == LIMITING)
  {
    pid->iout += pid->ki * pid->e;
  }
  else
  {
    if (fabsf(pid->e) <= pid->imaxout2)
    {
      pid->iout = pid->iout + pid->ki * pid->e;
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

  pid->dout = pid->kd * (pid->e - pid->last_e);
  pid->out = pid->iout + pid->pout + pid->dout;
  if (pid->out > pid->maxout)
  {
    pid->out = pid->maxout;
  }
  if (pid->out < -pid->maxout)
  {
    pid->out = -pid->maxout;
  }
  pid->last_e = pid->e;
}

void PID_Reset(PID_t *pid)
{
  if (pid == NULL)
    return;
  pid->e = 0.0f;
  pid->last_e = 0.0f;
  pid->pout = 0.0f;
  pid->iout = 0.0f;
  pid->dout = 0.0f;
  pid->out = 0.0f;
}

