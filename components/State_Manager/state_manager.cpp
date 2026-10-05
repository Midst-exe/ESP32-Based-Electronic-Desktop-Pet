
// #include "wifi_state_private.h"

#include "state_manager.h"
#include "esp_event.h"
#include "event_manager.h"

#include "esp_log.h"

static const char* TAG = "state_manager";


// 初始化配网状态为：空闲
static wifi_config_status_t g_wifi_config_status = WIFI_CONFIG_STATUS_IDLE;


// 初始化网络状态为：未连接
static wifi_status_t g_wifi_status = WIFI_DISCONNECTED;

// 全局IP地址 初始化为空  0.0.0.0
char ipv4_addr[16] = {0};


// 配网状态访问
wifi_config_status_t wifi_config_get_status(){
    return g_wifi_config_status;
}

// 查询网络状态
wifi_status_t wifi_get_status(){
    return g_wifi_status;
}








// 更新配网状态  回调
static void wifi_config_status_change_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){
    // 更新配网状态
    g_wifi_config_status = *(wifi_config_status_t*)event_data;
    ESP_LOGI(TAG,"已经修改状态%d",g_wifi_config_status);

}


// 初始化状态管理器
void state_manager_init(){
    // 注册事件监听器，监听 WIFI_CONFIG_EVENT_STATUS_CHANGED_REQUIRE 事件
    esp_err_t ret = esp_event_handler_instance_register(
        WIFI_CONFIG_EVENT,
        WIFI_CONFIG_EVENT_STATUS_CHANGED_REQUIRE,
        &wifi_config_status_change_handler,
        NULL,
        NULL
    );

    if(ret != ESP_OK){
        ESP_LOGE("State_Manager", "State Manager 初始化失败: %s", esp_err_to_name(ret));
    }
}

