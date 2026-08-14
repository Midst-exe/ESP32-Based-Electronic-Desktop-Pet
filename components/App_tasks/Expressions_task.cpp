/**
 * @file Expressions_task.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 动画任务示例
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
#include "Expressions.h"
#include "Irisoled.h"

static const char *TAG = "Expressions_task";


// 解析表情函数
Expressions_presets get_Expression(Expressions_presets_enum Expression_index){
	Expressions_presets expression_preset = Normal;
	switch (Expression_index)
	{
	case Expressions_presets_enum::Normal :    { expression_preset = Normal;    break; }
	case Expressions_presets_enum::Blink :     { expression_preset = Blink;     break; }
	case Expressions_presets_enum::Happy :     { expression_preset = Happy;     break; }
	case Expressions_presets_enum::Sad :       { expression_preset = Sad;       break; }
	case Expressions_presets_enum::Angry :     { expression_preset = Angry;     break; }
	case Expressions_presets_enum::Furious :   { expression_preset = Furious;   break; }
	case Expressions_presets_enum::Bored :     { expression_preset = Bored;     break; }
	case Expressions_presets_enum::Surprised : { expression_preset = Surprised; break; }
	case Expressions_presets_enum::Scared :    { expression_preset = Scared;    break; }
	case Expressions_presets_enum::Worried :   { expression_preset = Worried;   break; }
	case Expressions_presets_enum::Sleepy :    { expression_preset = Sleepy;    break; }
	case Expressions_presets_enum::Focused :   { expression_preset = Focused;   break; }
	case Expressions_presets_enum::Alert :     { expression_preset = Alert;     break; }
	case Expressions_presets_enum::Lost :      { expression_preset = Lost;      break; }
	case Expressions_presets_enum::Confused :  { expression_preset = Confused;  break; }
	case Expressions_presets_enum::Look_left : { expression_preset = Look_left; break; }
	case Expressions_presets_enum::Look_right :{ expression_preset = Look_right;break; }
	case Expressions_presets_enum::Look_up :   { expression_preset = Look_up;   break; }
	case Expressions_presets_enum::Look_down : { expression_preset = Look_down; break; }
	case Expressions_presets_enum::Wink_left : { expression_preset = Wink_left; break; }
	case Expressions_presets_enum::Wink_right :{expression_preset = Wink_right;break; }
	case Expressions_presets_enum::Charging_start :{expression_preset = Charging_start; break; }
	case Expressions_presets_enum::Low_battery :{expression_preset = Low_battery; break; }
	case Expressions_presets_enum::Full_battery :{expression_preset = Full_battery; break; }
	default:
		break;}
	return expression_preset;
}


void Expressions_task(void *arg)
{
  	ESP_LOGI(TAG, "Task Start");

	// 初始化I2C总线
    Init_I2C();
	// 创建 ssd1306_t 类型句柄 与 设备物理地址绑定OLED_ADDR_3D_96、配置初始化等
	ssd1306_t Oled_0;
	ssd1306_init(&Oled_0,OLED_ADDR_3D_96,bus_handle);
	// 初始化Expressions对象：  donghua
	// 注意：初始化时，已经将动画设置为循环播放；若未设置每帧延迟，则默认为200ms
//*******************测试动画为 Focused*******************
	Expressions donghua = {Focused}; 
    donghua.start();
    // 配置动画对象

    while (1)
    {

     // 放置具体内容App_tasks()
      donghua.update(&Oled_0);

      vTaskDelay(pdMS_TO_TICKS(1000)); // 主动出让cpu，防止饥饿

    }
}
