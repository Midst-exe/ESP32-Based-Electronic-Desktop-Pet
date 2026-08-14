/*
    ssd1306_framebuffer.cpp 
    主要是为了让 底层驱动 和 绘画层 解耦，本层主要提供一个
    画布buffer，而非一个bit这样直接传进 OLED 。  
*/

#include "ssd1306_framebuffer.h"

void ssd1306_clear(ssd1306_t *dev, uint8_t color) {
    // 如果传进来的是黑色，就把 buffer 全部填 0x00；如果是白色（全亮），填 0xFF
    uint8_t fill_val = (color == OLED_COLOR_WHITE) ? 0xFF : 0x00;
    
    // 使用 C 标准库内存覆盖整个逻辑画布
    ssd1306_clear_buffer(dev, fill_val);
}

void ssd1306_set_pixel(ssd1306_t *dev, int x, int y, uint8_t color) {
    uint32_t width;
    uint32_t high;
    // 安全边界检查。防止上层画图越界导致单片机内存非法改写（Crash）
    if ( dev->oled_addr == OLED_ADDR_3D_91 ) {
            width = SSD1306_WIDTH_91;
            high = SSD1306_HEIGHT_91;
        }
    else{ // 否则为0.96寸
            width = SSD1306_WIDTH_96;
            high = SSD1306_HEIGHT_96;
        }

    if (x < 0 || x >= width || y < 0 || y >= high) {
        return; 
    }

    // 【核心数学细节】：
    // SSD1306 的显存结构中：
    // 每 8 个垂直像素点组成 1 个字节 ，8行 每行128像素点，组成 1 page。
    // 整个屏幕高 64 or 32 像素，被切成了 8 个 Page (0 ~ 7) 或者4 page(0-3page)。

    // 计算公式拆解：
    // y / 8         -> 确定当前坐标落在第几个 Page (哪一行的字节)
    // (y / 8) * 128 -> 因为每行有 128 列，计算出当前 Page 的起始数组索引
    // x             -> 在当前 Page 内，向右偏移 x 个字节

    // 所以这一行计算的是某像素点所在字节的位置
    int index = x + (y / 8) * width;

    // y % 8  -> 确定我们要控制的是这 1 个字节（8个比特位）里的哪一个 bit
    uint8_t bit_pos = y % 8;

    // 硬件基础  位操作
    // 根据颜色进行位操作 (Bitwise Operations)
    if (color == OLED_COLOR_WHITE) {
        // 点亮像素：使用按位或 '|='，把对应的比特位置 1，其余位保持不变
        dev->buffer[index] |= (1 << bit_pos);
    } else {
        // 熄灭像素：使用按位与和取反 '&= ~'，把对应的比特位置 0，其余位保持不变
        dev->buffer[index] &= ~(1 << bit_pos);
    }
}

void ssd1306_flush(ssd1306_t *dev) {
    // 物理层交界点：直接调用驱动层的整屏刷新 API
    // 此时硬件层才会通过 I2C 真正开始搬运这 1024 字节
    ssd1306_update_screen(dev);
}