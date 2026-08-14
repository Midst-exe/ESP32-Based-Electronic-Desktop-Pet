/**
 * @file UART_Transmit.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 用于实现UART向OLED打印文本。支持中英文，未知字符打印'*'
 *          固定设备为  OLED_ADDR_3D_91  0.91寸
 * @version 0.1
 * @date 2026-08-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "App_tasks.h"
#include <stdio.h>
#include "BSP.h"
#include "ssd1306_graphics.h"


#define TX_Text_data_buffer_size 64


static const char *TAG = "UART_Transmit_task";



void check_and_display_uart(ssd1306_t *dev){
    // 设置一个接收缓冲区用于暂存文字内容
    uint8_t RX_Text_data_Buffer[Buffer_size];
    // 设置一个发送缓冲区用于暂存文字内容
    uint8_t TX_Text_data_buffer[TX_Text_data_buffer_size]; // 大小为64B

    // 取出Text_data长度  注意：留 1 字节给 '\0'，所以最多读 Buffer_size - 1
    int len = uart_read_bytes(UART_NUM_0, RX_Text_data_Buffer, Buffer_size - 1, 20);

    // 末尾补 '\0'
    if(len > 0) {
        // 拼接发送给PC的信息 
        snprintf((char*)TX_Text_data_buffer, TX_Text_data_buffer_size, "ESP: Successful Recieved %d Byte!", len);
        // 向PC端返回信息
        uart_write_bytes(UART_NUM_0, TX_Text_data_buffer, TX_Text_data_buffer_size);

    	ESP_LOGI(TAG, "接收成功！");

        RX_Text_data_Buffer[len] = '\0';
        ssd1306_graphics_clear(dev,0x00); // 清除画板
        graphics_update(dev);  //上传到屏幕
        
    	ESP_LOGI(TAG, "************************ 清除屏幕结束 ************************");


        // 调用graphics打印
        draw_string(dev,(const char *)RX_Text_data_Buffer, 0, 0);
        graphics_update(dev);  //上传到屏幕


    	ESP_LOGI(TAG, "************************ 打印文本完成 ************************");

    }

}


void UART_Transmit_task(void *arg){
  	ESP_LOGI(TAG, "Task Start！");

	// 初始化内容
    ssd1306_t Oled_0;
	ESP_ERROR_CHECK(ssd1306_init(&Oled_0,OLED_ADDR_3D_91,bus_handle));

	// oled_test();
    // 开机亮屏显示
	ssd1306_graphics_clear(&Oled_0, 0xff);
    ssd1306_flush(&Oled_0); // 推送上屏
    vTaskDelay(pdMS_TO_TICKS(3000));
	

    while (1)
    {
     // 放置具体内容App_tasks()
      check_and_display_uart(&Oled_0);
      vTaskDelay(pdMS_TO_TICKS(1000)); // 主动出让cpu，防止饥饿
    }
}

