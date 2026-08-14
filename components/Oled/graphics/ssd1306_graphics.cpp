/**
 * @file ssd1306_graphics.h
 * @author Midst-exe (Midst_exe@hotmail.com)
 * @brief SSD1306 OLED 图形绘制接口、文字渲染灯。基于SSD1306 Page格式的渲染方式
 *         含有  draw  以及 graphic等 
 * @version 0.1
 * @date 2026-07-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 // 其中所用位图、文字描述等资源在resource下
#include "ssd1306_graphics.h"
#include <stdlib.h>
#include <string.h>
#include "fonts_resource.h"

// 测试用
// #include "esp_log.h"
// static const char *TAG = "graphics层";


// 文字描述信息，声明在fonts_resource.h中
extern const uint8_t glyph_bitmap[];
extern const glyph_dsc_t glyph_dsc[];
extern const txt_cmap_t cmaps[];
extern const uint16_t unicode_list_3[];
extern const uint16_t unicode_list_5[];

// cmaps 的数量
#define CMAPS_COUNT 6

// 欢迎词
const char *Welcome = "Hello World!";
    

void ssd1306_graphics_clear(ssd1306_t *dev, uint8_t color){
    ssd1306_clear(dev,color);
}

void ssd1306_draw_pixel(ssd1306_t *dev, int x, int y, uint8_t color) {
    // 直接套用之前写好的底层显存映射函数
    ssd1306_set_pixel(dev, x, y, color);
}

void ssd1306_draw_line(ssd1306_t *dev, int x0, int y0, int x1, int y1, uint8_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; // 误差变量err

    while (1) {
        ssd1306_draw_pixel(dev, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void ssd1306_draw_rect(ssd1306_t *dev, int x, int y, int width, int height, bool is_filled, uint8_t color) {
    if (is_filled) {
        // 实心矩形：双重循环，按行书写，直接在区域内涂色
        for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                ssd1306_draw_pixel(dev, i, j, color);
            }
        }
    } else {
        // 空心矩形：调用画线 API 画出 4 条边界线
        ssd1306_draw_line(dev, x, y, x + width - 1, y, color);                  // 顶边
        ssd1306_draw_line(dev, x, y + height - 1, x + width - 1, y + height - 1, color); // 底边
        ssd1306_draw_line(dev, x, y, x, y + height - 1, color);                  // 左边
        ssd1306_draw_line(dev, x + width - 1, y, x + width - 1, y + height - 1, color); // 右边
    }
}

void ssd1306_draw_circle(ssd1306_t *dev, int x0, int y0, int radius, bool is_filled, uint8_t color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        if (is_filled) {
            // 实心圆：利用对称性，在左右两段圆弧之间连线进行水平填充
            ssd1306_draw_line(dev, x0 - x, y0 + y, x0 + x, y0 + y, color);
            ssd1306_draw_line(dev, x0 - x, y0 - y, x0 + x, y0 - y, color);
            ssd1306_draw_line(dev, x0 - y, y0 + x, x0 + y, y0 + x, color);
            ssd1306_draw_line(dev, x0 - y, y0 - x, x0 + y, y0 - x, color);
        } else {
            // 空心圆：算出一个点的轨迹，利用 8 分对称性同时点亮 8 个点
            ssd1306_draw_pixel(dev, x0 + x, y0 + y, color);
            ssd1306_draw_pixel(dev, x0 - x, y0 + y, color);
            ssd1306_draw_pixel(dev, x0 + x, y0 - y, color);
            ssd1306_draw_pixel(dev, x0 - x, y0 - y, color);
            ssd1306_draw_pixel(dev, x0 + y, y0 + x, color);
            ssd1306_draw_pixel(dev, x0 - y, y0 + x, color);
            ssd1306_draw_pixel(dev, x0 + y, y0 - x, color);
            ssd1306_draw_pixel(dev, x0 - y, y0 - x, color);
        }

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}


void ssd1306_draw_bitmap_full(ssd1306_t *dev,const uint8_t *bitmap)
{
    // void *memcpy(void *dest, const void *src, size_t n);
    // 其中dest:目的内存起始地址 , src:原内存起始地址【不会修改原内存数据】 , n:要复制的字节数，单位：Byte )
    memcpy(
        dev->buffer.data(),     // 目标：Framebuffer
        bitmap,          // 来源：Flash中的Bitmap
        1024             // 一整屏数据
    );
}


void ssd1306_draw_bitmap_partial(ssd1306_t *dev,int x, int y, const uint8_t *bitmap, int w, int h)
{
    int start_page = y / 8; //起始page

    //下式有个技巧：计算某式子的向上取整，避免使用浮点数
    //通式： result = (a + b - 1) / b; 计算 a/b 的向上取整
    int pages = (h + 7) / 8; //计算需要的page数，向上取整
    // 逐页拷贝位图数据到显存缓冲区
    for(int page=0; page<pages; page++)
    {
        // 计算目标显存缓冲区的起始位置和源位图数据的起始位置
        //循环 dst = （起始page + 偏移page） * page大小128列 + 字节偏移_x
        int dst =(start_page+page) * SSD1306_WIDTH_96 + x;
        
        //这个是想要插入的位图的循环
        //循环 src = 偏移page * page大小128列
        int src = page*w;
        // memcpy(目的地址, 源地址, 拷贝长度)   
        // 这里的拷贝长度是位图宽度w，因为每页的高度是8像素，正好对应SSD1306的一个page
        memcpy( &dev->buffer[dst], &bitmap[src], w);
    }
}


uint32_t get_glyph_id(uint32_t unicode) {
    for(uint32_t i = 0; i < CMAPS_COUNT; i++) {
        const txt_cmap_t *cmap = &cmaps[i]; // 直接存储地址，而非下标
        
        // 测试用
        // ESP_LOGI(TAG,"############### 文字Unicode：%lu ################",unicode);

        //判断此 unicode 在哪个 cmap[]
        if(unicode < cmap->range_start) continue;
        if(unicode >= cmap->range_start + cmap->range_length) continue;
        
        // // 测试用 打印字形位置信息
        // get_txt_cmap_t_info(i);

        if(cmap->unicode_list == NULL) {
            // 连续范围
            return cmap->glyph_id_start + (unicode - cmap->range_start);
        } else {
            // 稀疏范围（unicode_list_3 或 unicode_list_5）
            const uint16_t *list = (i == 3 ? unicode_list_3 : unicode_list_5);// 具体对应cmaps[]的实际存储情况
            
            for(uint32_t j = 0; j < cmap->list_length; j++) {
                if(list[j] == (unicode - cmap->range_start)) {
                    return cmap->glyph_id_start + j;
                }
            }
        }
    }
    return 11; // 未找到，返回"*"
}


uint8_t draw_char(ssd1306_t *dev, uint32_t unicode, int16_t x, int16_t y) {
    uint32_t glyph_id = get_glyph_id(unicode);      // 获取glyph_id
    const glyph_dsc_t *d = &glyph_dsc[glyph_id];    // 获取其字形信息
    
// **************************** 测试用 ******************************
    // get_glyph_dsc_info(glyph_id);  // 打印glyph_dsc[glyph_id]信息
// **********************************************************
    
    if(d->box_w == 0 || d->box_h == 0) return 0;  // 空字符
    
    uint32_t byte_start = d->bitmap_index;
    uint8_t result = 0x01; // 起始位：0000|0001B 8位循环 位计数器，每8位重置


    //详细见README
    // 在画布上做两个循环，row外循环是行序号，col内循环是列序号，遍历字形内每一个像素点。 
    for(uint8_t row = 0; row < d->box_h; row++) {

        // 测试用
        // ESP_LOGI(TAG,"############### 行序号：%d ################",row);
        
        for(uint8_t col = 0; col < d->box_w; col++) {
            // 计算当前像素所在的字节位置
            uint32_t pos = byte_start + (row * (d->box_w) + col) / 8;

            result = (result >> 1) | (result << (8 - 1)); // 实现循环右移1位

            // 测试用
            // ESP_LOGI(TAG,"列序号：%d,当前像素所在字节位置：%lu,位计数器：%d",col,pos,result);

            if(glyph_bitmap[pos] & result) 
                ssd1306_draw_pixel(dev, x + col  + d->ofs_x - 1  , y + row + d->ofs_y + 2, OLED_COLOR_WHITE);
        }
    }
    return d->box_w;
}

// 打印字符串主要处理光标移动，然后循环调用draw_char()
void draw_string(ssd1306_t *dev,const char* str, int16_t x, int16_t y) {
    if(str == NULL) return;

    // 记录起始坐标
    int16_t cur_x = x;
    uint8_t step_x = 0; 

    while(*str) {
        uint32_t unicode = 0;
        // 简单 UTF-8 转 Unicode  （Uincode码点还原）
        unsigned char c = (unsigned char)*str; //存储首字节内容
        if(c < 0x80) {                    // ASCII 单字节
            unicode = c;
            // str自增1字节
            str++;} 
        // 首字节高三位为110，表示为2字节
        else if((c & 0xE0) == 0xC0) {     // 2字节 UTF-8
            // 首个字节取前5bit并向左移6位，为后续6bit腾出位置，第二个字节只取低6位。
            // 得出11位uincode，高位补0
            unicode = ((c & 0x1F) << 6) | (str[1] & 0x3F);
            str += 2;} 
        else if((c & 0xF0) == 0xE0) {     // 3字节 UTF-8（中文常用）
            unicode = ((c & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
            str += 3;} 
        else {                         // 其他情况跳过
            str++;
            continue;}
        
        // 自动换行逻辑 ,处理行溢出
        if((cur_x + step_x) > Line_Width){
            y += Font_high_Max; // LF：向下一行
            cur_x = 0 ;  // CR：光标回到最左侧。
            //以上两步配合完成回车换行动作，即：CRLF
        }

        // 打印单个字体
        step_x = draw_char(dev,unicode, cur_x, y) + 2 ; // 每个字符间距为："字符宽 + 2像素"

        cur_x += step_x;// 移动光标，步进1单位
    }
}

esp_err_t ssd1306_init(ssd1306_t *dev,uint8_t device_address,i2c_master_bus_handle_t bus_handle){
    // driver层初始化
    esp_err_t net = ssd1306_driver_init(dev,device_address,bus_handle);
    
    // 清屏
    ssd1306_clear(dev,0x00);
    draw_string(dev,Welcome,0,0);
    return net;
}

void graphics_update(ssd1306_t *dev){
    ssd1306_flush(dev);
}