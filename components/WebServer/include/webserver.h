
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdint.h>
#include "esp_event.h"
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

const size_t index_html_size = (index_html_end - index_html_start);

/**
 * @brief webserver事件初始化函数
 * 用于注册外部事件通知 Webserver初始化
 * @attention 应在wifi初始化动作前调用
 */
void httpd_server_event_init();

/**
 * @brief Webserver初始化
 * @attention 事件初始化失败时才能调用，否则禁止调用
 */
void https_server_Init();


#endif 
