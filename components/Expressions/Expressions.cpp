/***************************************************
  从Arudino框架向ESP-IDF原生驱动移植的IrisoledAnimation类

  按照乐鑫IDF框架编写的非阻塞动画播放类，支持PROGMEM存储的位图帧数据
  2026-07-1
  Expressions.cpp
****************************************************/
#include "Expressions.h"
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "ssd1306_graphics.h"


// ********************* 测试用 ************************
#include "esp_log.h"
static const char *TAG = "Expressions";
// *****************************************************



//定义一个构造函数，分别传入帧数据指针数组、帧数、
//帧数据是否存储在PROGMEM中、每帧的延迟时间数组、默认帧延迟时间和是否循环播放的参数
//同时初始化成员变量，包括帧数据指针数组、帧数、循环播放标志、当前帧索引、上一次更新时间、播放状态和回调函数指针
// PROGMEM-frames constructor
Expressions::Expressions(const Expressions_presets& express,//帧数据指针数组
                                     uint16_t frameDelay,//默认帧延迟时间
                                     bool loop) ://是否循环播放
  //初始化成员变量
  _framesPROG(express.frame.data()),//PROGMEM帧数据指针数组
  _delays(express.delays.data()),//每帧的延迟时间数组
  _fallbackDelay(frameDelay),//默认帧延迟时间
  _frameCount(express.frameCount),//帧数
  _loop(loop),//是否循环播放
  _current(0),//当前帧索引，初始化为起始帧
  _lastMillis(0),//上一次更新时间，初始化为0
  _running(false),//播放状态，初始化为false
  _onFrameChange(nullptr) //初始化回调函数指针设置为nullptr，初始化没有回调函数
{}


// Start animation  开始动画
void Expressions::start(uint8_t startFrame) {
  if (_frameCount == 0) return;//若帧数为0，则直接返回，不进行播放
  _current = (startFrame < _frameCount) ? startFrame : 0;//起始帧序号赋值，如果起始帧序号大于等于帧数，则设置为0
  _lastMillis = xTaskGetTickCount();//记录当前时间戳，作为上一次更新时间
  _running = true;//设置播放状态为true，表示动画正在播放

  //如果有回调函数，并且当前帧已经改变，则调用回调函数。
  //回调函数需要在主函数中定义，并且在创建Expressions对象后通过setFrameCallback方法设置。
  if (_onFrameChange) _onFrameChange(_current);
}

// Stop/pause
void Expressions::stop() {
  _running = false;
}

// Resume
void Expressions::resume() {
  if (_frameCount == 0) return;
  _lastMillis = xTaskGetTickCount();
  _running = true;
}

// Reset
void Expressions::reset() {
  _current = 0;
  _running = false;
  _lastMillis = 0;
}

// Set looping
void Expressions::setLoop(bool loop) {
  _loop = loop;
}

// Set uniform delay
void Expressions::setFrameDelay(uint16_t ms) {
  _fallbackDelay = ms;
}

// Set per-frame delays array
void Expressions::setDelays(const uint16_t* delays) {
  _delays = delays;
}

// Set callback 回调函数指针设置
void Expressions::setFrameCallback(FrameCallback cb) {
  _onFrameChange = cb;
}

// Query functions  查询功能
uint8_t Expressions::getCurrentFrame() const { return _current; }
uint8_t Expressions::getFrameCount() const { 
  
  return _frameCount; }
bool Expressions::isRunning() const { return _running; }

// 获取指向索引位图的指针，如果需要，处理 PROGMEM 指针数组
const unsigned char* Expressions::getFramePtr(uint8_t index) const {
  if (index >= _frameCount || _framesPROG == nullptr) return nullptr;// 越界与空指针检查
  return _framesPROG[index]; //返回位图指针
}


// --------------------- Implementation ---------------------
// 64*128 全尺寸动画播放函数
void Expressions::update(ssd1306_t *dev) {

// *****************************************************
  	ESP_LOGI(TAG,"进入update函数"); // 测试用 
// *****************************************************


  if (!_running || _frameCount == 0) return;  
  unsigned long now = xTaskGetTickCount(); // 获取当前滴答计时器 数值
  // get current bitmap pointer (handles PROGMEM pointer arrays)
  const unsigned char* bmp = getFramePtr(_current); // 获取位图地址
  if (bmp) {


// *****************************************************
  	ESP_LOGI(TAG,"当前bmp:%d", bmp); // 测试用 
// *****************************************************

    // 直接调用graphics层全图渲染
    ssd1306_draw_bitmap_full(dev,bmp);
	  graphics_update(dev); // 推送上屏
  }

  //获取当前帧的延迟时间，如果有每帧延迟数组，则使用对应索引的延迟时间，否则使用默认延迟时间
  uint16_t delayMs = (_delays != nullptr) ? _delays[_current] : _fallbackDelay;

    //判断是否已经过了当前帧的延迟时间，如果是，则更新到下一帧
  if ((unsigned long)(now - _lastMillis) >= (unsigned long)delayMs) {

// *****************************************************
  	ESP_LOGI(TAG,"当前时间:%d", (unsigned long)(now)); // 测试用 
  	ESP_LOGI(TAG,"上次刷新时间:%d", (unsigned long)(_lastMillis)); // 测试用 
  	ESP_LOGI(TAG,"当前帧时间差:%d", (unsigned long)(now - _lastMillis)); // 测试用 
// *****************************************************

    _lastMillis = now;   // 重置上次更新时间
    uint8_t prev = _current;  // 
    _current++;//刷新帧指针
    //判断是否已经到达最后一帧，如果是，则根据是否循环播放来决定下一帧的索引
    if (_current >= _frameCount) {
      //这里_loop是一个布尔值，表示是否循环播放，如果是，则将当前帧索引重置为0，从头开始播放
      //所以不是“  _loop--； ”减去循环次数  
      if (_loop) _current = 0; //若循环，置当前帧为0，重新开始播放
      else { _current = _frameCount - 1; _running = false; }//否则停止播放，当前帧索引为最后一帧，播放状态为false
    }
    //如果有回调函数，并且当前帧已经改变，则调用回调函数。
    //回调函数需要在主函数中定义，并且在创建Expressions对象后通过setFrameCallback方法设置。
    //回调函数的参数是当前帧的索引，可以根据需要在回调函数中执行相应的操作。
    if (_onFrameChange && _current != prev) _onFrameChange(_current);
  }
}
