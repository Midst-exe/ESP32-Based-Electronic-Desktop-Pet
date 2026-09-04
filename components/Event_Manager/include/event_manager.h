/**
 * @file event_manager.h
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 全局事件管理总线 以及状态
 * @version 0.1
 * @date 2026-08-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

// 一般队列长度
#define EVENT_QUEUE_LENGTH 10

extern QueueHandle_t g_wifi_init_task_event_queue;


/* ---------------- 声明事件结构体 ---------------- */

typedef struct {
    esp_event_base_t base; // 事件基，如 WIFI_EVENT 或 MY_WEB_EVENT
    int32_t id;            // 事件 ID
    void *data;            // 附带数据
} event_t;
/* -------------------------------------------------------- */


/* ---------------- 声明所有事件基 ---------------- */
ESP_EVENT_DECLARE_BASE(WIFI_CONFIG_EVENT);  // Wi-Fi 配网相关事件基
ESP_EVENT_DECLARE_BASE(OTA_EVENT);          // OTA 升级相关事件基
ESP_EVENT_DECLARE_BASE(SENSOR_EVENT);       // 传感器数据相关事件基

/* -------------------------------------------------------- */

/* ---------------- 定义各类事件基对应的 ID ---------------- */

// 配网事件 ID
typedef enum {
    WIFI_CONFIG_EVENT_GOT_CREDENTIALS = 0,
    WIFI_CONFIG_EVENT_WEBSERVER_INIT, // 触发webserver_init初始化事件
    WIFI_CONFIG_EVENT_CONNECTING, // 正在连接
    WIFI_CONFIG_EVENT_SUCCESS, // 连接成功
    WIFI_CONFIG_EVENT_GET_IP, // 已获取ip
    WIFI_CONFIG_EVENT_PERMITED_CONNECT, // 准许连接wifi
    WIFI_CONFIG_EVENT_PASSWORD_ERROR, // 密码错误
    WIFI_CONFIG_EVENT_SSID_NO_FOUND, // 找不到wifi
    WIFI_CONFIG_EVENT_DISCONNECT_OTHER_REASON, // 其他原因导致未连接
    WIFI_CONFIG_EVENT_MAX_CONNECT_FAILED, // 多次重连失败
    WIFI_CONFIG_EVENT_FAIL, // 配网失败
} wifi_config_event_id_t;

// OTA 事件 ID
typedef enum {
    OTA_EVENT_START = 0,
    OTA_EVENT_PROGRESS,
    OTA_EVENT_SUCCESS,
    OTA_EVENT_FAIL,
} ota_event_id_t;

// 传感器事件 ID
typedef enum {
    SENSOR_EVENT_DATA_READY = 0,
    SENSOR_EVENT_ALARM,
} sensor_event_id_t;
/* -------------------------------------------------------- */


/* ---------------- 定义各事件对应的数据结构 ---------------- */

// 定义SSID和psw缓冲区大小
#define SSID_SIZE 32
#define PWD_SIZE 64


typedef struct {
    char ssid[SSID_SIZE];
    char password[PWD_SIZE];
} wifi_credentials_t;

typedef struct {
    int progress_percent; // OTA 进度百分比
} ota_progress_t;

/* -------------------------------------------------------- */



/* ----------------------- 定义各状态 ----------------------- */



/* -------------------------------------------------------- */


















#ifdef __cplusplus
}
#endif

#endif 