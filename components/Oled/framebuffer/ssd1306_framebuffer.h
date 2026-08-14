/*
    ssd1306_framebuffer.h 
    主要是为了让 底层驱动 和 绘画层 解耦，本层主要提供一个
    画布buffer，而非一个bit这样直接传进 OLED 。  
*/


#ifndef SSD1306_FRAMEBUFFER_H
#define SSD1306_FRAMEBUFFER_H

#include "ssd1306_driver.h"

// 颜色常量定义
#define OLED_COLOR_BLACK  0x00 // 灭（黑色）
#define OLED_COLOR_WHITE  0xff // 亮（白色）

// 图形层核心 API
/**
 * @brief 更新 SSD1306 OLED 显示器屏幕内容
 * 
 * @param dev OLED 设备句柄
 * @param color 填充颜色 (0x00 表示全黑，0xff 表示全白)
 */
void ssd1306_clear(ssd1306_t *dev, uint8_t color);

/**
 * @brief 设置像素点函数
 * 
 * @param dev 屏幕句柄
 * @param x 水平坐标 (0 ~ 127)
 * @param y 垂直坐标 (0 ~ 63)
 * @param color 填充颜色 (0x00 表示黑色，0xFF 表示白色)
 */
void ssd1306_set_pixel(ssd1306_t *dev, int x, int y, uint8_t color);


/**
 * @brief 上传画板的图像到屏幕
 * 
 * @param dev 屏幕句柄
 */
void ssd1306_flush(ssd1306_t *dev);

#endif // SSD1306_FRAMEBUFFER_H