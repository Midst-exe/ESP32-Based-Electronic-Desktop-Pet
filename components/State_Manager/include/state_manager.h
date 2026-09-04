
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


/**
 * @brief 全局配网状态
 */
typedef enum
{
    WIFI_CONFIG_STATUS_IDLE = 0, // 空闲
    WIFI_CONFIG_STATUS_CONNECTING, // 正在连接
    WIFI_CONFIG_STATUS_SUCCESS, // 连接成功
    WIFI_CONFIG_STATUS_PASSWORD_ERROR, // 密码错误
    WIFI_CONFIG_STATUS_SSID_NO_FOUND, // 未找到wifi
    WIFI_CONFIG_STATUS_DISCONNECT_OTHER_REASON, // 其他原因导致未连接
    WIFI_CONFIG_STATUS_GET_IP, // 获取到IP地址
    WIFI_CONFIG_STATUS_MAX_CONNECT_FAILED, // 多次重连失败
    WIFI_CONFIG_STATUS_FAILED // 配网失败
} wifi_config_status_t;

/**
 * @brief 全局网络状态
 */
typedef enum
{
    WIFI_CONNECTED = 0, // wifi已连接
    WIFI_DISCONNECTED, // wifi未连接
    WIFI_OFF // wifi未打开
}wifi_status_t;


/**
 * @brief 全局ip地址
 */
static char ipv4_addr[16];


#endif
