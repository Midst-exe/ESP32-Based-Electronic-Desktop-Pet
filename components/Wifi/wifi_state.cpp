
#include "wifi_state_private.h"

#include "state_manager.h"
#include "esp_event.h"
#include "event_manager.h"

// 初始化配网状态为：空闲
static wifi_config_status_t g_wifi_config_status = WIFI_CONFIG_STATUS_IDLE;
// 初始化网络状态为：未连接
static wifi_status_t g_wif_status = WIFI_DISCONNECTED;



// 状态更新函数 只向wifi组件暴露
void update_wifi_config_status(wifi_config_status_t new_status){
    if (wifi_manager_get_status() == new_status) return;   // 状态没变，不发事件

    g_wifi_config_status = new_status;   // 更新全局状态

    // 根据新状态发送对应事件
    switch (new_status) {
    case WIFI_CONFIG_STATUS_CONNECTING:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_CONNECTING, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_SUCCESS:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_SUCCESS, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_PASSWORD_ERROR:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_PASSWORD_ERROR, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_SSID_NO_FOUND:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_SSID_NO_FOUND, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_MAX_CONNECT_FAILED:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_MAX_CONNECT_FAILED, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_DISCONNECT_OTHER_REASON:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_DISCONNECT_OTHER_REASON, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_GET_IP:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_GET_IP, NULL, 0, 0);
        break;
    case WIFI_CONFIG_STATUS_FAILED:
        esp_event_post(WIFI_CONFIG_EVENT, WIFI_CONFIG_EVENT_FAIL, NULL, 0, 0);
        break;
    default:
        break;
    }
}




