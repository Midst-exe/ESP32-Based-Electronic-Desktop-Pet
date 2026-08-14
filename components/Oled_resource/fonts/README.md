# 字体生成
** 具体的工作流如下： ** 
 
 
 输入文字 -> Unicode -> cmap[]查找其glyph_id -> glyph_dsc[glyph_id] -> 读取glyph_map点阵数据 -> 绘制到屏幕

**在bitmap中的数据存储形式为 uint_8 ,也就是8bit（1字节）为一单位**
**其对应索引 bitmap_index 单位也是1B**


对于**unicode_list**这样的数组
    unicode_list_3和unicode_list_5,内部存储的是文字unicode和本数组属性的.range_start差值。

    这个差值的下标就是文字的glyph_id，再由glyph_id去获取该文字字形描述glyph_dsc[glyph_id]，其中就存储有.bitmap_index，即文字glyph_bitmap[]下标。