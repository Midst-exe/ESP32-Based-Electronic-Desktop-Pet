/**
 * @file BSP.h
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-08-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */


 #ifndef BSP_H
 #define BSP_H

#include "driver/i2c_types.h"
#include "driver/i2c_master.h"
#include "driver/uart.h"
// 在ssd_1306_driver.h中声明
// #define OLED_ADDR_0 0x3C  // 0x3C 所指OLED
// #define OLED_ADDR_1 0x3D  // 0x3D 所指OLED

#define Buffer_size 1024


// 声明总线 句柄
extern i2c_master_bus_handle_t bus_handle;


/**
 * @brief 初始化I2C
 * 
 * @param bus_handle I2C总线句柄
 */
 void Init_I2C();


/**
 * @brief 初始化以上所有板载资源，直接在main中调试用
 * 
 * @param I2C_bus_handle I2C总线句柄
 */
 void Init_board();

#endif
