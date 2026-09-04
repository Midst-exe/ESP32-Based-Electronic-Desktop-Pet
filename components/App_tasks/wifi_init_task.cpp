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

static const char *TAG = "wifi_Init_Task";

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
