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


/**
 * @brief 初始化wifi
 * 
 * @attention 系统初始化用
 */
void wifi_Init();


/**
 * @brief wifi连接
 * 
 * @param wifi_info 传进ssid,pwd
 */
void wifi_connect_handler(wifi_credentials_t* wifi_info);

#endif
