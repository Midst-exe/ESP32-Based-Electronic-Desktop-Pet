//板级初始化

#include "BSP.h"

#include "driver/i2c_master.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "Hardware_Pin.h"

#define Buffer_size 1024

static const char *TAG = "BSP";


// I2C 配置
// 定义  i2c总线 句柄
 i2c_master_bus_handle_t bus_handle;

void Init_I2C(){
    // ==============================  初始化I2C  ==============================
    // I2C master bus specific configurations
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = I2C_NUM_0,              /*!< I2C port number, `-1` for auto selecting, (not include LP I2C instance) */
        .sda_io_num = I2C_SDA,                /*!< GPIO number of I2C SDA signal, pulled-up internally */
        .scl_io_num = I2C_SCL,                /*!< GPIO number of I2C SCL signal, pulled-up internally */
        .clk_source = I2C_CLK_SRC_DEFAULT,        /*!< Clock source of I2C master bus */
        .glitch_ignore_cnt = 7,            /*!< If the glitch period on the line is less than this value, it can be filtered out, typically value is 7 (unit: I2C module clock cycle)*/
        .intr_priority = 0,                    /*!< I2C interrupt priority, if set to 0, driver will select the default priority (1,2,3). */
        .trans_queue_depth = 0,             /*!< Depth of internal transfer queue, increase this value can support more transfers pending in the background, only valid in asynchronous transaction. (Typically max_device_num * per_transaction)*/
        .flags = {
            .enable_internal_pullup = 1, 
            .allow_pd = 0
        }
    };

    // 由i2c_new_master_bus()创建I2C管理对象返回bus_handle，并判断是否成功
    if( i2c_new_master_bus(&i2c_bus_config, &bus_handle) != ESP_OK) {
		ESP_LOGE(TAG,"错误：%S","无法由i2c_new_master_bus()创建I2C管理对象返回bus_handle");
		return ;
	}
    /*  ***********  初始化OLED   device   这一部分在ssd1306_driver中已经初始化过 *********************

    // ==============================  初始化OLED   device  ==============================
    // ==============================  初始化OLED_0  ==============================
    // 初始化配置
    i2c_device_config_t oled_0_config = { 
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,    //!< Length of I2C device address 
        .device_address = OLED_ADDR_0,                    //!< I2C device address 
        .scl_speed_hz = 100000,                    // !< SCL clock frequency for this device 
        .scl_wait_us = 0,                      // !< Timeout value. (unit: us). Please note this value should not be so small that it can handle stretch/disturbance properly. If 0 is set, that means use the default reg value
        .flags{ .disable_ack_check = false }      // !< Disable ACK check. If this is set false, that means ack check is enabled, the transaction will be stopped and API returns error when nack is detected. 
        };

    // 由 i2c_master_bus_add_device()添加设备并返回oled_config和oled_handle，并判断是否成功
    if( i2c_master_bus_add_device(bus_handle, &oled_0_config, &oled_0_handle) != ESP_OK) {
		ESP_LOGE(TAG,"错误：%S","无法由 i2c_master_bus_add_device()添加设备并返回oled_config和oled_handle");
		return ;
	}
      ***********  这一部分在ssd1306_driver中已经初始化过 *********************    */
    ESP_LOGI(TAG,"I2C初始化完成");
}


// UART配置

void Init_UART(){
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
    

    // ==============================  TODO  ==============================

    // 2. 设置引脚  UART_NUM_0中默认的TX为GPIO_43,RX为GPIO_44  ，UART_PIN_NO_CHANGE表明保持默认  宏定义UART_PIN_NO_CHANGE = -1;
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0,43,44, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // 3. 给UART_NUM_0安装驱动
    // 设置环形缓冲区
    const int uart_buffer_size = Buffer_size; // 设置 1KB 的接收缓冲区

    if( uart_is_driver_installed(UART_NUM_0)==false )  
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, uart_buffer_size, 0, 0, NULL, 0));
}


// 声明初始化函数

void Init_board(){
	 Init_I2C();
     Init_UART();
};
