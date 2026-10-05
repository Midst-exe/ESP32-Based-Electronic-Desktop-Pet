/**
 * @file App_tasks.h
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 各个应用任务声明
 * @version 0.1
 * @date 2026-08-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef APPS_TASKS_H
#define APPS_TASKS_H

#include "BSP.h"
#include "ssd1306_graphics.h"


/**
 * @brief 实现UART向OLED打印文本  的任务。支持中英文，未知字符打印'*'
 *  固定向设备OLED 0.91寸屏幕 
 * @param arg 
 */
void UART_Transmit_task(void *arg);

/**
 * @brief 播放动画任务
 * 
 * @param arg 
 */
void Expressions_task(void *arg);

/**
 * @brief AP配网初始化任务
 * 
 * @param arg 
 */
void Wi_fi_init_task(void *arg);


#endif
