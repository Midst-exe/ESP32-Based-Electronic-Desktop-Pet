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
  
  I2C_BUS
   SCL   -> GPIO_16 
   SDA   -> GPIO_15

  SSD 1306 OLED Pin Connection:
   SCL   -> GPIO_16 
   SDA   -> GPIO_15

*/


/* --------------- I2S PIN CONFIG -------------- */
#define I2S_WS   GPIO_NUM_12
#define I2S_SCK  GPIO_NUM_11
#define I2S_SD    GPIO_NUM_8   // 麦克风输入 (Data In)
#define I2S_DOUT  GPIO_NUM_10   // 喇叭输出 (Data Out) 
#define BOOT_BUTTON GPIO_NUM_0  //按键输入语音
/* --------------- OLED PIN CONFIG I2C -------------- */
#define I2C_SDA GPIO_NUM_15
#define I2C_SCL GPIO_NUM_16

// #define OLED_ADDR_0 0x3C  // 0x3C 所指OLED

//动作控制
#define motor_IN1   GPIO_NUM_4
#define motor_IN2   GPIO_NUM_5
#define motor_IN3   GPIO_NUM_6
#define motor_IN4   GPIO_NUM_7


#endif 