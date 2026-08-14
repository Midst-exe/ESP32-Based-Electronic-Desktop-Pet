**Oled组件中的功能说明**

**dirver -> framebuffer -> graphics**

同时，resource向graphics层提供图片和文字bitmap，以及Expressions帧数据等

在graphics层中实现文字打印功能

而表情动画则作为**单独组件**放置在component下

# 操作指南 （均由graphics层向外提供函数接口）
1. 在graphics层 先创建ssd1306_t类型句柄,指针

2. 将该句柄利用ssd1306_init()初始化;此时会显现亮屏动画文字等。

3. 在gtaphics层中有许多绘图函数，利用其绘图；或者利用ssd1306_draw_bitmap_full()将Oled_resourse中存储的bitmap推送画在画布上； 或者利用draw_string()等函数写出文字。  

4. 在画布上绘画完成后，在graphics层中有   void graphics_update(ssd1306_t *dev);  记得推送上屏幕！

