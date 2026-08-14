//动作组件

#ifndef ACTION_H
#define ACTION_H

#include <driver/gpio.h>
#include "Hardware_Pin.h"
#include "esp_rom_sys.h"
#include "esp_log.h" 


//配置电机控制引脚为输出模式
void setup_motor_pins();

//基础动作函数
void stop_motors();
void move_forward();
void move_backward();
void turn_left();
void turn_right();

// 激活电机
// @param act_code 动作模式 (1:前进  2:后退   3:左转   4:右转   0:停止)
void start_motors(int act_code);

//关闭电机
void stop_motors_now();


#endif