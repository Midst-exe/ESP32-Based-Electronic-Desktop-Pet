#ifndef SSD1306_DRIVER_H
#define SSD1306_DRIVER_H

#include <iostream>
#include <vector>

#include <stdint.h>
#include "driver/i2c_types.h"
#include "driver/i2c_master.h"
#include <esp_err.h>



// ************************* OLED 0.96 ****************************
#define OLED_ADDR_3D_96 0x3D  // 0x3D 所指OLED 0.96
#define SSD1306_WIDTH_96         128
#define SSD1306_HEIGHT_96        64
#define SSD1306_BUFFER_SIZE_1024   ((SSD1306_WIDTH_96 * SSD1306_HEIGHT_96) / 8) // 1024 字节

// ************************* OLED 0.91 ****************************
#define OLED_ADDR_3D_91 0x3C  // 0x3C 所指OLED 0.91
#define SSD1306_WIDTH_91         128
#define SSD1306_HEIGHT_91        32
#define SSD1306_BUFFER_SIZE_512   ((SSD1306_WIDTH_91 * SSD1306_HEIGHT_91) / 8) // 512 字节


#define I2C_SCL_400KHz        400000 // I2C 时钟频率 400KHz


// 驱动句柄结构体
typedef struct {
    i2c_master_dev_handle_t i2c_dev; // ESP-IDF I2C 设备句柄
    uint8_t oled_addr ; // 显式说明设备地址
    std::vector<uint8_t> buffer; // 显存缓冲区（全屏刷新用）
} ssd1306_t;

/** 
 * @brief 初始化 SSD1306 OLED 显示器
 * @param dev OLED 设备句柄
 * @param device_address 设备地址
 * @param bus_handle I2C 总线句柄
 * @return ESP_OK 表示成功，其他值表示失败
 */
esp_err_t ssd1306_driver_init(ssd1306_t *dev,uint8_t device_address,i2c_master_bus_handle_t bus_handle);

/** @brief 向 SSD1306 OLED 显示器发送命令
 *  @param dev 指向 SSD1306 设备结构体的指针
 *  @param cmd 要发送的命令,具体命令请参考 SSD1306 数据手册  P28 : Command Table
 *  @return ESP_OK 表示成功，其他值表示错误
 */
esp_err_t ssd1306_write_cmd(ssd1306_t *dev, uint8_t cmd);

/** @brief 向 SSD1306 OLED 显示器发送数据
 *  @param dev 指向 SSD1306 设备结构体的指针
 *  @param data 要发送的数据指针
 *  @param size 要发送的数据大小
 *  @return ESP_OK 表示成功，其他值表示错误
 */
esp_err_t ssd1306_write_data(ssd1306_t *dev, const uint8_t *data, size_t size);

/** @brief 更新 SSD1306 OLED 显示器屏幕
 *  @param dev 指向 SSD1306 设备结构体的指针
 *  @return ESP_OK 表示成功，其他值表示错误
 */
esp_err_t ssd1306_update_screen(ssd1306_t *dev);


/** @brief 清除 SSD1306 OLED 显示器缓冲区
 * 
 *  @param dev 指向 SSD1306 设备结构体的指针
 *  @param color 填充颜色 (0x00 表示全黑，0xFF 表示全白)
 */
void ssd1306_clear_buffer(ssd1306_t *dev, uint8_t color);

#endif // SSD1306_DRIVER_H