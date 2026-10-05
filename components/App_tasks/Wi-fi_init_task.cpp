/**
 * @file Wi-fi_init_task.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 实现wifi  AP网页配网  任务
 * @attention 务必初始化任务循环后开启
 * 
 * @version 0.1
 * @date 2026-08-10
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "App_tasks.h"
#include <stdio.h>
#include "BSP.h"

#include "Wifi.h"
#include "webserver.h"

#include "state_manager.h"
#include "event_manager.h"

/*
    编写流程：
    1. 初始化内容
    2. 放置具体内容App_tasks()
    3. 在App_tasks.h文件中声明任务函数

*/

static const char *TAG = "Wi-fi_init_task";

// 初始化存储wifi配网信息指针

static wifi_credentials_t* wifi_info_data;

// 接收webserver传回的配网信息
void wifi_connect_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){
    wifi_info_data = (wifi_credentials_t*) event_data;

    wifi_config_status_t g_wifi_config_status = WIFI_CONFIG_STATUS_IDLE;
    // 测试
    ESP_LOGI(TAG,"进入回调函数wifi_connect_handler，承接wifi_info  %d",*wifi_info_data);

    
    // 修改配网状态
    // 触发状态改变事件 通知State_manager组件
    esp_event_post(WIFI_CONFIG_EVENT,
            WIFI_CONFIG_EVENT_STATUS_CHANGED_REQUIRE,
            &g_wifi_config_status,
            sizeof(wifi_config_status_t),
            0);  
}


void Wi_fi_init_task(void *arg){
  	ESP_LOGI(TAG, "Wi-fi_init_task Task Start");

    // 状态初始化加入系统初始化
    state_manager_init();
    // 注册WIFI_CONFIG_EVENT中事件WIFI_CONFIG_EVENT_PERMITED_CONNECT
    // 对应回调函数wifi_connect_handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
                            WIFI_CONFIG_EVENT,
                            WIFI_CONFIG_EVENT_PERMITED_CONNECT,
                            &wifi_connect_handler,
                            NULL,
                            NULL
                    )
    );

	// 1. 初始化内容
	wifi_Init();
    https_server_Init();

    // 承接当前配网状态
    wifi_config_status_t g_wifi_config_status;

    while (1)
    {
        g_wifi_config_status = wifi_config_get_status();

        switch (g_wifi_config_status){
        case WIFI_CONFIG_STATUS_CONNECTING:  // 正在连接，拒绝访问
            ESP_LOGW("WiFi", "WiFi正在连接，拒绝重复配置!");
            break;

        case WIFI_CONFIG_STATUS_IDLE: // 等待重连
            // // 尝试重新连接
            reconnect_times++; // 重连次数加1 
            if(wifi_info_data != NULL) wifi_connect(wifi_info_data);
            break;

        default:
            break;
        }
     // 2. 放置具体内容App_tasks()
      

      vTaskDelay(pdMS_TO_TICKS(1000)); // 主动出让cpu，防止饥饿

    }
}
