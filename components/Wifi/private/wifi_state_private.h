
#ifndef WIFI_STATE_PRIVATE_H
#define WIFI_STATE_PRIVATE_H

#include "Wifi.h"
#include "state_manager.h"

/**
 * @brief 获取wifi配网状态
 * 
 * @return wifi_config_status_t 
 */
wifi_config_status_t wifi_manager_get_status(void);

/**
 * @brief 状态更新函数 
 * @attention 只向wifi组件暴露,同时发布对应事件通知
 */
void update_wifi_config_status(wifi_config_status_t new_status);




#endif // WIFI_STATE_PRIVATE_H