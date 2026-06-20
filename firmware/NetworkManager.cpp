// NetworkManager.cpp
#include <WiFi.h>
#include "driver/i2s.h"
#include <WebSocketsClient.h>
#include "NetworkManager.h"
#include "Hardware_Pin.h" 
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display; // 引用主程序定义的显示屏对象

WebSocketsClient webSocket;

void init_Network() {
    // 1. 在屏幕上打字
    display.clearDisplay();
    display.setTextSize(2); //数值表示放大倍率
    display.setTextColor(SSD1306_WHITE); //确保颜色被设置
    display.setCursor(0, 20);//光标打印位置
    display.println("Connecting WiFi...");
    display.display();


    // 2. 联网逻辑
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // 3. 联网成功反馈
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("WiFi Connected!");
    display.display();
    delay(1500);

    //串口打印
    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}


/* =========================================================
   WebSocket 事件回调 :当某个“事件发生”时，系统自动调用你提前写好的函数。
   ========================================================= */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch(type)
  {
    case WStype_CONNECTED:
      Serial.println("WebSocket Connected");
      break;

    case WStype_DISCONNECTED:
      Serial.println("WebSocket Disconnected");
      break;

    case WStype_TEXT:
    {
      Serial.printf("Received Text: %s\n", payload);
      
      // 1. 创建 JSON 解析对象
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON解析失败: ");
        Serial.println(error.c_str());
        return;
      }

      // 2. 提取动作码、持续时间和表情码
          /* JSON 格式为    {
              "content": "你的回复内容",
              "act_code": "动作编号",
              "duration": "持续时间",
              "express_code":"你的表情编码"
            }  */

      int act_code = doc["act_code"] | 0;   // 如果没有该字段，默认为 0
      int duration = doc["duration"] | 0;
      int express_code = doc["express_code"] | -1; //默认 -1 表示没收到
      // 3. 执行逻辑 
      //处理动作
      if (act_code > 0) {
        Serial.printf("执行动作: %d, 持续时间: %d ms\n", act_code, duration);
        // 调用非阻塞动作函数
        move_non_blocking(act_code, duration); 
      }
      //处理表情
      if (express_code >= 0 && express_code <= 21){
        // 强转：将 int 转换为 Emotion 枚举
        Emotion target_emo = static_cast<Emotion>(express_code);
        Serial.printf("触发表情:express_code_%d\n", express_code);

        // 调用Expressions.cpp封装好的触发函数
        trigger_emotion(target_emo);
      }
      break;
    }

    case WStype_BIN:
        //  收到二进制音频流，直接写入 I2S 播放 
      if (!isListening) {//先判断是否正在录音
        size_t bytes_written;
        // 将收到的数据推送到 I2S,portMAX_DELAY 确保完整播放
        i2s_write(I2S_NUM_0, payload, length, &bytes_written, portMAX_DELAY);
      }
      break;

    default:
      break;
  }
}