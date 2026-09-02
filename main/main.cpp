
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "esp_event.h"
#include "App_tasks.h"

#include "BSP.h"
#include "Hardware_Pin.h"
#include "Wifi.h"


// 该板配置 tick_Hz = 1KHz 有 1 tick = 1 ms

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

  OLED_ADDR_0 0x3C  // 0x3C 所指OLED
  OLED_ADDR_1 0x3D  // 0x3D 所指OLED

*/


static const char *TAG = "MAIN";

static esp_err_t init_system_resources(void){
  esp_err_t ret = ESP_OK;

  // 打开事件循环
  ret = esp_event_loop_create_default();
  //初始化板载资源
	board_Init();
  // 初始化wifi，并执行连接逻辑
  wifi_Init();

  // dpp_enrollee_init();

	return ret;



}



extern "C" void app_main(void)
{
	if (init_system_resources() != ESP_OK) {
    	    ESP_LOGE(TAG, "系统初始化失败，挂起程序！");
        	return;
    }

    xTaskCreate(
        UART_Transmit_task,  //修改
        "UART_Transmit_task",  //修改
        4096,
        NULL,
        5,
        NULL);
}


