// 声明各个状态名及其含义
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


/* ----------------------- 定义各状态 ----------------------- */

/**
 * @brief 全局配网状态
 */
typedef enum
{
    WIFI_CONFIG_STATUS_IDLE = 0, // 空闲 OR 等待重连
    WIFI_CONFIG_STATUS_CONNECTING, // 正在连接
    WIFI_CONFIG_STATUS_SUCCESS, // 连接成功
    WIFI_CONFIG_STATUS_GET_IP, // 获取到IP地址
    WIFI_CONFIG_STATUS_FAILED // 配网失败
} wifi_config_status_t;

/**
 * @brief 配网失败原因
 */
typedef enum
{
    WIFI_CONFIG_FAIL_REASON_UNKNOWN = 0, // 未知原因
    WIFI_CONFIG_FAIL_REASON_PASSWORD_ERROR, // 密码错误
    WIFI_CONFIG_FAIL_REASON_SSID_NO_FOUND, // 未找到WLAN
    WIFI_CONFIG_FAIL_REASON_MAX_CONNECT_FAILED // 连接失败次数过多
} wifi_config_fail_reason_t;


/**
 * @brief 全局网络状态
 */
typedef enum
{
    WIFI_CONNECTED = 0, // wifi已连接
    WIFI_DISCONNECTED, // wifi未连接
    WIFI_OFF, // wifi未打开
}wifi_status_t;

// 全局ip地址
extern char ipv4_addr[16];

/* -------------------------------------------------------- */


/**
 * @brief 初始化状态管理器
 * @attention 该函数会注册事件监听器，监听 WIFI_CONFIG_EVENT_STATUS_CHANGED_REQUIRE 以及 配网状态改变 事件
 *          在系统初始化中调用该函数，以确保状态管理器能够正确处理配网状态的变化。
 * @return void
 */
void state_manager_init();


/**
 * @brief 查询配网状态
 * 
 * @return wifi_config_status_t 
 */
wifi_config_status_t wifi_config_get_status();

/**
 * @brief 查询wifi状态
 * 
 * @return wifi_status_t 
 */
wifi_status_t wifi_get_status();




#endif  // STATE_MANAGER_H
