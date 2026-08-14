/**
 * @file test_task.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 测试任务
 * @version 0.1
 * @date 2026-08-10
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "App_tasks.h"
#include <stdio.h>
#include "BSP.h"

/*
    编写流程：
    1. 初始化内容
    2. 放置具体内容App_tasks()
    3. 在App_tasks.h文件中声明任务函数

*/

static const char *TAG = "test_Task";

void test_task(void *arg)
{
  	ESP_LOGI(TAG, "Task Start");

	// 1. 初始化内容
	

    while (1)
    {

     // 2. 放置具体内容App_tasks()
      

      vTaskDelay(pdMS_TO_TICKS(1000)); // 主动出让cpu，防止饥饿

    }
}
