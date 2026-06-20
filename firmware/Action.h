#ifndef ACTION_H
#define ACTION_H

#include <Arduino.h>
#include "Hardware_Pin.h"


/* =========================
   电机基础控制函数
   ========================= */
void set_motor_logic(int in1, int in2, int in3, int in4);

void stop_motors();
void move_forward();
void move_backward();
void turn_left();
void turn_right();

/* =========================
   非阻塞动作控制
   ========================= */

// 动作控制（1前进 2后退 3左转 4右转 0停止）
void move_non_blocking(int act_code, int duration);

// 在 loop 中持续调用，用于检测时间并停止电机
void update_motor_task();

#endif