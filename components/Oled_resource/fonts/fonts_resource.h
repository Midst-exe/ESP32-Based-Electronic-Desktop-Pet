/**
 * @file fonts_resource.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-07-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef FONTS_RESOURCE_H
#define FONTS_RESOURCE_H

#include <stdint.h>

#define Font_high_Max 16 // 字高最大值
#define Font_width_Max 16 // 字宽最大值

/**
 * @brief 字形描述结构体
 * 
 */
typedef struct {
    uint32_t bitmap_index; // 对应字库里的 .bitmap_index
    uint32_t adv_w;        // 光标步进
    uint8_t box_w;         // 字形宽
    uint8_t box_h;         // 字形高
    int8_t ofs_x;          // X偏移
    int8_t ofs_y;          // Y偏移
} glyph_dsc_t;


/**
 * @brief 字形描述存储位置结构体
 * 
 */
typedef struct {
        uint32_t range_start;       // 从哪个 Unicode 编码开始
        uint32_t range_length;      //连续多少字符
        uint32_t glyph_id_start;    // 对应glyph数组的起始位置
        const uint16_t * unicode_list;      // unicode字符列表
        uint32_t list_length;       // 列表长度
        // 映射表类型：LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY 适用于连续的字符范围
    } txt_cmap_t;



// 文字数据声明
extern const uint8_t glyph_bitmap[];

// 文字属性数据和索引
extern const glyph_dsc_t glyph_dsc[];

// utf-8编码的unicode列表
extern const uint16_t unicode_list_3[];

// utf-8编码的unicode列表
extern const uint16_t unicode_list_5[];

extern const txt_cmap_t cmaps[];

extern const uint16_t cmaps_count;


/**
 * @brief 串口打印字形信息
 * 
 * @param glyph_dsc_index glyph_dsc[]下标
 */
void get_glyph_dsc_info(uint32_t glyph_dsc_index);

/**
 * @brief 串口打印字形存储信息
 * 
 * @param cmaps_index  cmap[]下标
 */
void get_txt_cmap_t_info(uint8_t cmaps_index);

#endif
