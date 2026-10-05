/**
 * @file WiFi.h 关于初始化，链接，重连等逻辑
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef WIFI_H
#define WIFI_H
// 定义SSID和psw缓冲区大小
#define SSID_SIZE 32
#define PSW_SIZE 64


#include <stdio.h>

extern uint8_t reconnect_times; // 重连次数重置
#define MAX_RECONNECT_TIMES 3  // 最大重连次数


/**
 * @brief 初始化wifi
 * 
 * @attention 系统初始化用
 */
void wifi_Init();




// 修改为普通函数，
// wifi连接回调

/**
 * @brief wifi连接功能
 * @attention 由wifi_task调用
 * 
 * @param wifi_info wifi配网信息指针
 */
void wifi_connect(wifi_credentials_t* wifi_info);

#endif
