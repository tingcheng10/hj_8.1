# 任务七 代码规范总结
## 一、格式规范

### 1.1 ctrl+alt+l自动格式化，tab2空格，左大括号独立成行，删除.c文件多余的include guard

## 二、命名规范

### 2.1 函数名

| 改前 | 改后 |  理由  |
|---|---|---|
| `RIGHTHANDLE()` | `Motor_RightHandle()` | 全大写是宏的写法，加模块前缀，驼峰来写函数名 |
| `ERRORHANDLE()` | `Motor_ErrorHandle()` | 全大写是宏的写法，加模块前缀，驼峰来写函数名 |
| `CAN_FILTER_Init()` | `CAN_FilterInit()` | 模块前缀+驼峰，不用下划线 |
| `CAN_cmd_chassis()` | `CAN_CmdChassis()` | 模块前缀+驼峰，不用下划线 |
| `send_mail_box ` | `send_mailbox ` | mailbox是完整单词 |

### 2.2 结构体成员

| 改前 | 改后 | 理由 |
|---|---|---|
| `temp` | `temperature_c` | 补上全称和单位 °C |
| `speed` | `speed_rpm` | 补上单位 rpm|
| `angle` | `angle_raw` | 补上单位说明为编码器刻度 |
| `current` | `current_raw` | 表明是电流原始值 |
| `e` | `error` | 补上全称，说明是该次误差 |
| `last_e` | `last_error` | 补上全称，说明是上次误差 |
| `maxout` | `out_max` | 输出限幅，out 是输出、max 是最大 |
| `last_time` | `last_rx_time_ms` | 表明上次串口接收时间，单位ms |
| `now` | `now_tick_ms` | 表明是该次时间记录，注明单位ms |
| `back` | `feedback` | 反馈全称 |
| `out` | `output` | 改成全称电流输出量 |
| `pout` / `iout` / `dout` | `p_out` / `i_out` / `d_out` |pid前缀区分，out说明用途输出|
| `imaxout1` | `integral_max` | 积分全称，max表示积分最大限幅 |
| `imaxout2` | `integral_separate_threshold` | 积分分离阈值的全称 |


### 2.3 局部变量

| 改前 | 改后 | 理由 |
|---|---|---|
| `flag1` | `servo_dir` |看不出用途，改为servo_dir明确表示舵机转动方向|
| `servo_angle` | `servo_angle_deg` | 加上单位说明，角度单位° |
| `key_tick` | `key_last_tick_ms` | 说明为上次判断记录时间，且单位为ms |
| `flag2` | `led_fade_dir` |看不出用途，改为led_fade_dir明确表示led变化|
| `motor_par` | `motor_parameter` |补全名称，说明是参数|


### 2.4 类型名

| 改前 | 改后 | 理由 |
 |---|---|---|
 | `motor_par` | `MotorParam_t` | motor前缀表示电机参数，_t表示类型名 |

### 2.5 宏定义

| 原写法 | 改为宏 |
|---|---|
| `limiting` | `LIMITING` |
| `separating` | `SEPARATING` |
| `500u` | `SERVO_PWM_MIN` |
| `2500u` | `SERVO_PWM_MAX` |
| `180` | `SERVO_ANGLE_MAX_DEG` |
| `20` | `SERVO_ANGLE_STEP_DEG` |
| `30` | `KEY_DEBOUNCE_MS` |
| `100` | `MOTOR_OFFLINE_TIMEOUT_MS` |
| `150.0f`| `SPEED_TARGET_MAX_RPM` |
| `1000`| `LED_PWM_MAX` |
| `20` | `LED_PWM_STEP` |
| `2000`| `REPORT_PERIOD_MS` |
修改理由：

- 数字直接写在代码里无法看出含义
- 宏定义替换魔法数之后修改容易

### 2.6 形参

| 改前 | 改后 |
|---|---|
| `maxout` | `out_max` |
| `back` | `feedback` |
修改理由： 

- 与结构体成员一致，易读懂


### 2.7 命名原则

宏定义用大写；  
函数名要用模块前缀加驼峰，下划线分词；  
命名变量时要区分用途，单位，时间；  

## 三、注释规范

### 3.1 函数头注释

- `@brief` 说明这个函数做什么
- `@param` 说明参数含义与单位
- `@retval` 说明返回值  

| 文件 | 函数 |
|---|---|
| `PID.c` | `PID_Init`、`PID_Calc`、`PID_Reset` |
| `can_motor.c` | `CAN_FilterInit`、`CAN_Init`、`CAN_CmdChassis`、`Motor_ParseRxData`、`Motor_RightHandle`、`Motor_ErrorHandle`、`HAL_CAN_RxFifo0MsgPendingCallback` |
| `freertos.c` | `StartkeyTask`、`StartmotorTask`、`StartuartrxTask`、`StartledTask`、`StartuarttxTask`、`HAL_GPIO_EXTI_Callback`、`HAL_UART_RxCpltCallback` |

## 四、对比改动

改动在main里，7.1代码在original  
使用命令行可查看代码改动

```bash
git diff original main               #(命令行分页)按q退出
git --no-pager diff original main    #（不分页）
git diff --stat original main        #（看改动的文件）
git log --oneline original           #（看original的历史修改）
git log --oneline                    #(看所有的git commit -m)
```
