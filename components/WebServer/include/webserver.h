
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdint.h>
#include "esp_event.h"
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

const size_t index_html_size = (index_html_end - index_html_start);


/**
 * @brief web服务器的回调函数
 * 
 * @param event_handler_arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
void http_server_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data);


#endif 
