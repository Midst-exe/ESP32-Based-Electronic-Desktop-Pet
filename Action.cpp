// action.cpp
#include <Arduino.h>
#include "Hardware_Pin.h"

/* =========================================================
   * 电机驱动核心函数
   * @param in1, in2, in3, in4 对应真值表电平
   ========================================================= */
void set_motor_logic(int in1, int in2, int in3, int in4) {
  // 硬件保护：在切换任何动作前，先进入极短的死区时间（全低电平）
  digitalWrite(motor_IN1, LOW);
  digitalWrite(motor_IN2, LOW);
  digitalWrite(motor_IN3, LOW);
  digitalWrite(motor_IN4, LOW);
  delay(2); // 2ms 死区时间，防止反向电动势冲击驱动板

  // 写入新的逻辑电平
  digitalWrite(motor_IN1, in1);
  digitalWrite(motor_IN2, in2);
  digitalWrite(motor_IN3, in3);
  digitalWrite(motor_IN4, in4);
}

//  非阻塞式动作功能函数
static int current_act = 0;
static unsigned long act_start_time = 0;
static unsigned long act_duration = 0;

// 停止运动/待机/(0,0,0,0)
void stop_motors() {
  set_motor_logic(0, 0, 0, 0);
}

// 前进 (0101)
void move_forward() {
  set_motor_logic(0, 1, 0, 1);
}

// 后退 (1010)
void move_backward() {
  set_motor_logic(1, 0, 1, 0);
}

// 原地左转 (1001)
void turn_left() {
  set_motor_logic(1, 0, 0, 1);
}

// 原地右转 (0110)
void turn_right() {
  set_motor_logic(0, 1, 1, 0);
}

/**
 * 带时间控制的运动函数
 * @param act_code 动作模式 (1:前进  2:后退   3:左转   4:右转   0:停止)
 * @param duration 持续运行的时间(ms)
 */
// 这个函数只负责“下达命令”，不负责“等待完成”
void move_non_blocking(int act_code, int duration) {
    current_act = act_code;
    act_start_time = millis();
    act_duration = duration;
    Serial.printf("执行动作码: %d\n", act_code);
    // 立即开启电机
    switch(act_code) {
        case 1: move_forward();  break;
        case 2: move_backward(); break;
        case 3: turn_left();     break;
        case 4: turn_right();    break;
        default: stop_motors();  current_act = 0; return;
    }
}

//需要放在 loop 里不停执行，负责“按时关闭”电机
void update_motor_task() {
    if (current_act > 0) {
        if (millis() - act_start_time >= act_duration) {
            stop_motors();
            current_act = 0; // 动作结束
            Serial.println("动作时间到，电机停止");
        }
    }
}