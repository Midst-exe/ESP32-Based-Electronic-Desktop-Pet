/**
 * @file ssd1306_graphics.h
 * @author Midst-exe (Midst_exe@hotmail.com)
 * @brief SSD1306 OLED 图形绘制接口。基于SSD1306 Page格式的渲染方式
 * @version 0.1
 * @date 2026-07-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef SSD1306_GRAPHICS_H
#define SSD1306_GRAPHICS_H

#include "ssd1306_framebuffer.h"

#define Line_Width 128  // 屏幕宽

/**
 * @brief 初始化OLED
 * 
 * @param dev ssd1306_t类型的屏幕句柄
 * @param device_address 设备地址
 * @param bus_handle 总线句柄
 * @return esp_err_t 
 */
esp_err_t ssd1306_init(ssd1306_t *dev,uint8_t device_address,i2c_master_bus_handle_t bus_handle);

/**
 * @brief 清空 SSD1306 OLED 显示器屏幕内容
 * 
 * @param dev OLED 设备句柄
 * @param color 填充颜色 (0x00 表示全黑，0xff 表示全白)
 */
void ssd1306_graphics_clear(ssd1306_t *dev, uint8_t color);

// 基础绘图 API 声明
/**
 * @brief Pixel (单个原子、像素点操作)
 * 
 * @param dev 屏幕句柄
 * @param x 横坐标
 * @param y 纵坐标
 * @param color 填充颜色 0x00 (黑色) 或 0xFF (白色)
 */
void ssd1306_draw_pixel(ssd1306_t *dev, int x, int y, uint8_t color);

/**
 * @brief Line (Bresenham 直线算法)
 * 
 * @param dev 屏幕句柄
 * @param x0 起始点横坐标
 * @param y0 起始点纵坐标
 * @param x1 终点横坐标
 * @param y1 终点纵坐标
 * @param color 填充颜色 0x00 (黑色) 或 0xFF (白色)
 */
void ssd1306_draw_line(ssd1306_t *dev, int x0, int y0, int x1, int y1, uint8_t color);

/**
 * @brief 绘制矩形
 * 
 * @param dev 屏幕句柄
 * @param x 左上角横坐标
 * @param y 左上角纵坐标
 * @param width 宽度
 * @param height 高度
 * @param is_filled 是否填充
 * @param color 填充颜色
 */
void ssd1306_draw_rect(ssd1306_t *dev, int x, int y, int width, int height, bool is_filled, uint8_t color);

/**
 * @brief 绘制圆 (Bresenham 中点画圆算法：利用 8 分对称性快速铺满)
 * 
 * @param dev 屏幕句柄
 * @param x0 圆心横坐标
 * @param y0 圆心纵坐标
 * @param radius 半径
 * @param is_filled 是否填充
 * @param color 填充颜色
 */
void ssd1306_draw_circle(ssd1306_t *dev, int x0, int y0, int radius, bool is_filled, uint8_t color);

/**
 * @brief 将一张全尺寸128x64屏幕Bitmap复制到Framebuffer
 *
 * @param dev       SSD1306设备对象
 * @param bitmap    完整屏幕位图数据
 *
 * Bitmap格式要求:
 *      宽度: 128 pixel
 *      高度: 64 pixel
 *      大小: 1024 Byte
 *
 * 数据格式必须与SSD1306 Framebuffer一致:
 *
 *      Page0 : 128 Byte
 *      Page1 : 128 Byte
 *      ...
 *      Page7 : 128 Byte
 *
 */
void ssd1306_draw_bitmap_full(ssd1306_t *dev,const uint8_t *bitmap);


/**
 * @brief 脏区域刷新，部分刷新
 * 
 * @param dev 设备句柄
 * @param x 脏区域起始X坐标
 * @param y 脏区域起始Y坐标
 * @param bitmap 位图数据
 * @param w 位图宽度
 * @param h 位图高度
 */
void ssd1306_draw_bitmap_partial(ssd1306_t *dev,int x, int y, const uint8_t *bitmap, int w, int h);

/*  ============== 以下为打印文字用 【未知字符返回'*'的信息】==============   */
/**
 * @brief 根据 Unicode 查找 glyph_id
 * 
 * @param unicode 
 * @return uint32_t glyph_id
 */
uint32_t get_glyph_id(uint32_t unicode);

/**
 * @brief 确定某一坐标，写出文字
 * 
 * @param dev 设备句柄
 * @param unicode 文字unicode码
 * @param x 已知横坐标
 * @param y 已知纵坐标
 */
uint8_t draw_char(ssd1306_t *dev, uint32_t unicode, int16_t x, int16_t y);

/**
 * @brief 打印字符串
 *  
 * @param str 字符串指针
 * @param x 起始位置横坐标x
 * @param y 起始位置纵坐标y
 */
void draw_string(ssd1306_t *dev,const char* str, int16_t x, int16_t y);

/**
 * @brief 画板上传到屏幕【绘制层】
 * 
 * @param dev 设备句柄
 */
void graphics_update(ssd1306_t *dev);

#endif // SSD1306_GRAPHICS_H
