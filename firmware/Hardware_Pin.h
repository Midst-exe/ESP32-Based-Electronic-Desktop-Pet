#ifndef HARDWARE_PIN_H
#define HARDWARE_PIN_H

/*
  ESP32S3 I2S Microphone -> WebSocket Binary Stream
  Hardware:
    ESP32S3
    INMP441 I2S Microphone
    L298N
    SSD 1306 0.96 OLED
    MAX98357A 功放

  INMP441 Microphone Pin Connection:
    SCK  -> GPIO_11
    WS   -> GPIO_12
    SD   -> GPIO_8
    麦克风VCC只能接3.3v，禁止接5V。
    共芯片地

  MAX98357A 功放 Pin Connection:
    LRC  -> GPIO_12
    BCLK -> GPIO_11
    DIN  -> GPIO_10
    BCLK -> 置空
    SD   -> 置空

  MX1508 Pin Connection:
   IN1   -> GPIO_4
   IN2   -> GPIO_5
   IN3   -> GPIO_6
   IN4   -> GPIO_7

  SSD 1306 OLED Pin Connection:
   SCL   -> GPIO_16 
   SDA   -> GPIO_15

*/


/* --------------- I2S PIN CONFIG -------------- */
#define I2S_WS   12
#define I2S_SCK  11
#define I2S_SD    8   // 麦克风输入 (Data In)
#define I2S_DOUT  10   // 喇叭输出 (Data Out) 
#define BOOT_BUTTON 0  //按键输入语音
/* --------------- OLED PIN CONFIG -------------- */
#define OLED_SDA 15
#define OLED_SCL 16

//动作控制
#define motor_IN1   4
#define motor_IN2   5
#define motor_IN3   6
#define motor_IN4   7


#endif 