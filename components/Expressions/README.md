# 关于Expressions组件的使用说明

Expressions_presets中的动画结构体
''' 
// 在Expressions_presets已经定义了一个表情帧组类
// 同时在Expressions_presets.h中存储有组帧数据
struct Expressions_presets
{
    int frameCount;                 // 帧数
    std::vector<uint16_t> delay;    // 帧延迟数组
    std::vector<const unsigned char*> frame; // 帧数据
};
'''

注意：初始化时，已经将动画设置为循环播放；若未设置每帧延迟，则默认为200ms。


**使用流程**
1. 初始化一个Expressions对象并初始化，在Irisoled.h中选择播放表情A
2. 使用Expressions的start方法配置动画播放状态 \__running 为 true
3. 在while中循环执行对象的update方法。


示例：



void test_task(void *arg)
{
  	ESP_LOGI(TAG, "Test Task Start");
	// OLED设备句柄
	// 创建 ssd1306_t 类型句柄 与 设备物理地址绑定、配置初始化等
	ssd1306_t Oled_0;
	ssd1306_init(&Oled_0,OLED_ADDR_0,bus_handle);
	// 初始化Expressions对象：  donghua
	// 注意：初始化时，已经将动画设置为循环播放；若未设置每帧延迟，则默认为200ms
	Expressions donghua = {Focused}; // 测试动画为 Focused
    donghua.start();

	ssd1306_graphics_clear(&Oled_0, 0xff);
    ssd1306_flush(&Oled_0); // 推送上屏
    vTaskDelay(pdMS_TO_TICKS(3000));

    while (1)
    {

  	    ESP_LOGI(TAG, "*********************** 测试动画***********************");
  	    ESP_LOGI(TAG, "*********************** 动画开始 ***********************");
		    donghua.update(&Oled_0); // 播放眨眼动画
        // vTaskDelay(pdMS_TO_TICKS(5000));
  	    ESP_LOGI(TAG, "*********************** 动画结束 ***********************");
        vTaskDelay(pdMS_TO_TICKS(1000)); // 别忘了让出CPU
    }
}



**上述while中也可测试单帧图片，以下可以直接替换循环体中内容**

  	    ESP_LOGI(TAG, "***********************  动画单帧播放  ***********************");
		ssd1306_draw_bitmap_full(&Oled_0,(uint8_t *)Irisoled::normal);  // 测试图片为Irisoled::normal
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));

		ssd1306_draw_bitmap_full(&Oled_0,(uint8_t *)Irisoled::wink_right); // 测试图片为Irisoled::wink_right
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));
  	    ESP_LOGI(TAG, "#######################  动画单帧结束  #######################");


**屏幕亮灭测试**
  	    ESP_LOGI(TAG, "***********************##### 屏幕亮暗测试 #####***********************");
  	    ESP_LOGI(TAG, "***********************##### 屏幕亮 #####***********************");

		  ssd1306_graphics_clear(&Oled_0, 0xff);
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));
  	    ESP_LOGI(TAG, "***********************##### 屏幕灭 #####***********************");

		  ssd1306_graphics_clear(&Oled_0, 0x00);
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(1000));
	
  	    ESP_LOGI(TAG, "***********************##### 屏幕亮暗测试结束 #####***********************");