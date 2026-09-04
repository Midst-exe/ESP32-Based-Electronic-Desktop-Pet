/**
 * @file System_init_task.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 编写系统初始化任务
 * @version 0.1
 * @date 2026-08-10
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "App_tasks.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "BSP.h"
#include "ssd1306_graphics.h"

#include "state_manager.h"
#include "event_manager.h"

#include "wifi.h"
#include "webserver.h"

/*  *********** 不要直接在本文件修改 ***********
    编写流程：
    1. 初始化内容
    2. 放置具体内容App_tasks()
    3. 在App_tasks.h文件中声明任务函数
    *********** 不要直接在本文件修改 ***********
*/
// 全局变量：全局事件队列与组件状态
QueueHandle_t g_wifi_init_task_event_queue = xQueueCreate(EVENT_QUEUE_LENGTH,sizeof(event_t));
// 初始化配网状态为：空闲
static wifi_config_status_t g_wifi_config_status = WIFI_CONFIG_STATUS_IDLE;

static const char *TAG = "Wifi_Init_Task";



// 配网状态更新函数
void update_wifi_config_status(wifi_config_status_t new_status){
    if (g_wifi_config_status == new_status) return;   // 状态没变
    g_wifi_config_status = new_status;   // 更新全局状态
}


void wifi_init_task(void *arg)
{
  	ESP_LOGI(TAG, "Wifi Init Task Start");

	// 1. 初始化内容
	wifi_Init();
    https_server_Init();

    while (1)
    {

     // 2. 放置具体内容App_tasks()
      

      vTaskDelay(pdMS_TO_TICKS(1000)); // 主动出让cpu，防止饥饿

    }
}
