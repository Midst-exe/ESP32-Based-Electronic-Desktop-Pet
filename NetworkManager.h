#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

/* =========================================================
   依赖库包含
   ========================================================= */
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "Expressions.h"  // 必须包含，因为要使用 Emotion 枚举类型

/* =========================================================
   外部变量声明 (extern)
   这些变量定义在别处（如 .cpp 或 .ino），但在网络模块中需要用到
   ========================================================= */
extern WebSocketsClient webSocket;
extern bool isListening; // 录音状态位，防止音频下行与上行冲突

// ...  Wfi  外部变量声明 ...
extern const char* WIFI_SSID;
extern const char* WIFI_PASS;

/* =========================================================
   函数声明 (Prototypes)
   ========================================================= */

/**
 * @brief 初始化网络连接
 * 包含 WiFi 握手逻辑及 OLED 状态实时反馈
 */
void init_Network();

/**
 * @brief WebSocket 事件处理回调
 * 负责解析服务器下发的 JSON 指令（动作/表情）及二进制音频流
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

/* =========================================================
   外部逻辑接口
   声明定义在主程序或其他模块中的控制函数，以便在 .cpp 中调用
   ========================================================= */
/**
 * @brief 非阻塞式动作执行函数（定义在电机控制模块）
 * @param code 动作编号
 * @param duration 持续时间
 */
void move_non_blocking(int code, int duration);

#endif