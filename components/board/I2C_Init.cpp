
#include "BSP.h"

#include "driver/i2c_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "Hardware_Pin.h"

static const char *TAG = "I2C_Init";


// I2C 配置
// 定义  i2c总线 句柄
 i2c_master_bus_handle_t bus_handle;

void I2C_Init(){
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
    ESP_LOGI(TAG,"初始化完成");
}
