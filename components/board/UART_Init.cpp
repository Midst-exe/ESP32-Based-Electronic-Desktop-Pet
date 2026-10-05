#include "BSP.h"

#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "Hardware_Pin.h"

#define Buffer_size 1024
static const char *TAG = "UART_Init";

void UART_Init(){
    // 1. 先配置uart参数
    uart_config_t uart_handle_config = {
        .baud_rate = 115200,                    
        .data_bits = UART_DATA_8_BITS,       /*!< UART byte size*/
        .parity = UART_PARITY_DISABLE,               /*!< UART parity mode*/
        .stop_bits = UART_STOP_BITS_1,         /*!< UART stop bits*/
        /* 当接收方处理速度跟不上发送方时，
            接收方可以通过硬件信号告诉发送方「先别发了」，
            等自己处理完再继续发。这比软件流控（XON/XOFF）更快、更可靠 */
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,    /*!< UART HW flow control mode (cts/rts)*/
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT,
        .flags = {
                .allow_pd = 0,
                .backup_before_sleep = 0
        }
    };

    // 写入配置参数
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0,&uart_handle_config));
    
    // 2. 设置引脚  UART_NUM_0中默认的TX为GPIO_43,RX为GPIO_44  ，UART_PIN_NO_CHANGE表明保持默认  宏定义UART_PIN_NO_CHANGE = -1;
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0,43,44, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // 3. 给UART_NUM_0安装驱动
    // 设置环形缓冲区
    const int uart_buffer_size = Buffer_size; // 设置 1KB 的接收缓冲区
    // 判断若未安装驱动则重新安装
    if( uart_is_driver_installed(UART_NUM_0)==false )  
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, uart_buffer_size, 0, 0, NULL, 0));

    ESP_LOGI(TAG,"资源初始化完成!");
    
}

