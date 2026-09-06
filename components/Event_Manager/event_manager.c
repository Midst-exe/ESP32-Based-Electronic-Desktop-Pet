
#include "event_manager.h"

// 一次性把所有的事件基内存分配好
ESP_EVENT_DEFINE_BASE(WIFI_CONFIG_EVENT);
ESP_EVENT_DEFINE_BASE(OTA_EVENT);
ESP_EVENT_DEFINE_BASE(SENSOR_EVENT);

// 普通全局变量
// 初始化配网状态为：空闲
wifi_config_status_t g_wifi_config_status = WIFI_CONFIG_IDLE;

// 普通全局变量
// 初始化网络状态为：未连接
wifi_status_t g_wif_status = WIFI_OFF;

// 全局IP地址 初始化为空  0.0.0.0
char ipv4_addr[16] = {0};