//板级初始化

#include "BSP.h"
#include "esp_log.h"

static const char *TAG = "BSP";


// 声明初始化函数
void board_Init(){
	 I2C_Init();
     UART_Init();
     nvs_Init();
    ESP_LOGI(TAG,"板级资源初始化完成!");
};
