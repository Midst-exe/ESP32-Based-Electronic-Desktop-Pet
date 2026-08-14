/***************************************************
  从Arudino框架向ESP-IDF原生驱动移植的IrisoledAnimation类

  按照乐鑫IDF框架编写的非阻塞动画播放类，支持PROGMEM存储的位图帧数据
  2026-07-1
  Expressions.cpp
  
  Expressions.h
  Non-blocking animation helper for Irisoled bitmaps (PROGMEM)
****************************************************/

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"   // 配置和控制数据传输通道（I2C/SPI/RGB接口等）
#include "esp_lcd_panel_vendor.h" // 各种特定屏幕芯片驱动（如 SSD1306, ST7789 等）
#include "esp_lcd_panel_ops.h"    // 屏幕的具体操作（如刷屏、清屏、反色等）
#include "Irisoled.h"
#include "ssd1306_graphics.h"


class Expressions {
public:

  //定义回调函数类型，用于在帧改变时通知外部
  typedef void (*FrameCallback)(uint8_t newIndex);

  /**
   * @brief 定义构造函数，初始化帧数据和相关参数
   * 
   * @param express 表情帧组类对象，声明在 Irisoled.h 中，存储在 PROGMEM 中
   * @param frameDelay 默认帧延迟时间（毫秒）【默认参数，选填】
   * @param loop 是否循环播放动画【默认参数，选填。若只修改loop，则frameDelay也要填入】
   */
  Expressions(const Expressions_presets& express,  // 见下方TAG注释
              uint16_t frameDelay = 200,
              bool loop = true);

  // control
  void start(uint8_t startFrame = 0);
  void stop();
  void resume();
  void reset();
  void setLoop(bool loop);
  void setFrameDelay(uint16_t ms);
  void setDelays(const uint16_t* delays);
  void setFrameCallback(FrameCallback cb);

  // query  查询函数
  uint8_t getCurrentFrame() const;
  uint8_t getFrameCount() const;
  bool isRunning() const;


  // 声明播放函数，传入屏幕句柄和绘制位置及尺寸参数
  void update(ssd1306_t *dev);

private:
  const unsigned char* getFramePtr(uint8_t index) const;

  const unsigned char* const* _framesPROG;
  bool _framesInPROGMEM;

  const uint16_t* _delays;
  uint16_t _fallbackDelay;

  uint8_t _frameCount;
  bool _loop;
  uint8_t _current;
  unsigned long _lastMillis;
  bool _running;

  FrameCallback _onFrameChange;
  
  

};

// ---------------------       TAG:   可选择的表情动画express                 ---------------------
// --------------------- 声明在Oled_resource/Expressions_bitmap/Irisoled.h ---------------------


#undef IR_READ_PTR_FROM_PROGMEM

#endif // EXPRESSIONS_H
