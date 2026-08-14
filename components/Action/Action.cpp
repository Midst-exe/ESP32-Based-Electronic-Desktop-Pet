// action.cpp
#include <driver/gpio.h>
#include "Hardware_Pin.h"
#include "esp_rom_sys.h"
#include "esp_log.h" 


static const char *TAG = "MotorControl"; // 日志标签，用于标识日志来源

/* =========================================================
   * 电机驱动核心函数
   * @param in1, in2, in3, in4 对应真值表电平
   ========================================================= */
void set_motor_logic(int in1, int in2, int in3, int in4) {
  // 硬件保护：在切换任何动作前，先进入极短的死区时间（全低电平）
  gpio_set_level(motor_IN1, 0);
  gpio_set_level(motor_IN2, 0);
  gpio_set_level(motor_IN3, 0);
  gpio_set_level(motor_IN4, 0);
  esp_rom_delay_us(2); // 2微秒 死区时间，防止反向电动势冲击驱动板

  // 写入新的逻辑电平
  gpio_set_level(motor_IN1, in1);
  gpio_set_level(motor_IN2, in2);
  gpio_set_level(motor_IN3, in3);
  gpio_set_level(motor_IN4, in4);
}


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





void setup_motor_pins() {
  // 配置电机控制引脚为输出模式
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE; // 禁用中断
  io_conf.mode = GPIO_MODE_OUTPUT;       // 设置为输出模式
  io_conf.pin_bit_mask = (1ULL << motor_IN1) | (1ULL << motor_IN2) | (1ULL << motor_IN3) | (1ULL << motor_IN4);// 设置要配置的引脚
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // 禁用下拉
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // 禁用上拉
  gpio_config(&io_conf);
}


// 激活电机
// @param act_code 动作模式 (1:前进  2:后退   3:左转   4:右转   0:停止)
void start_motors(int act_code){
      // 立即开启电机
    ESP_LOGI(TAG, "解析动作码: %d", act_code);
    switch(act_code) {
        case 1: {move_forward(); 
          ESP_LOGI(TAG, "执行   1:前进");
          break;}
        case 2: {move_backward();
          ESP_LOGI(TAG, "执行   2:后退");
          break;}
        case 3: {turn_left();     
          ESP_LOGI(TAG, "执行   3:左转");
          break;}
        case 4: {turn_right();    
          ESP_LOGI(TAG, "执行   4:右转");
          break;}
        default: {stop_motors(); return;}
    }

}

//关闭电机
void stop_motors_now() {
    stop_motors();
    ESP_LOGI(TAG, "电机已停止");
}

