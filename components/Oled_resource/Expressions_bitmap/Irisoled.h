/***************************************************
  Irisoled - Robotic OLED Eye Expressions Library
  Copyright (c) 2025 Chijindu-Orji Iseh-Ntah

  Licensed under the MIT License.
  See license.txt for details.

  Irisoled.h
****************************************************/

#ifndef IRISOLED_H
#define IRISOLED_H

#include <vector>
#include <cstdint>

// 单表情帧bitmap  存放在./risoled.cpp
namespace Irisoled {
    extern const unsigned char alert[] ;
    extern const unsigned char angry[] ;
    extern const unsigned char blink_down[] ;
    extern const unsigned char blink_up[] ;
    extern const unsigned char blink[] ;
    extern const unsigned char bored[] ;
    extern const unsigned char despair[] ;
    extern const unsigned char disoriented[] ;
    extern const unsigned char excited[] ;
    extern const unsigned char focused[] ;
    extern const unsigned char furious[] ;
    extern const unsigned char happy[] ;
    extern const unsigned char look_down[] ;
    extern const unsigned char look_left[] ;
    extern const unsigned char look_right[] ;
    extern const unsigned char look_up[] ;
    extern const unsigned char normal[] ;
    extern const unsigned char sad[] ;
    extern const unsigned char scared[] ;
    extern const unsigned char sleepy[] ;
    extern const unsigned char surprised[] ;
    extern const unsigned char wink_left[] ;
    extern const unsigned char wink_right[] ;
    extern const unsigned char worried[] ;
    extern const unsigned char battery_full[] ;
    extern const unsigned char battery_low[] ;
    extern const unsigned char battery[] ;
    extern const unsigned char left_signal[] ;
    extern const unsigned char logo[] ;
    extern const unsigned char mode[] ;
    extern const unsigned char right_signal[] ;
    extern const unsigned char warning[] ;
}

/*
    这里主要声明动画帧数据 Expressions_presets.cpp
*/
//  该结构体必须为  全局或静态生命周期
// 定义一个表情帧组类
struct Expressions_presets
{
    uint8_t frameCount;                 // 帧数
    std::vector<uint16_t> delays;    // 帧延迟数组
    std::vector<const unsigned char*> frame; // 帧数据数组
};

// 声明所有表情动画对象
extern Expressions_presets Normal;
extern Expressions_presets Blink;
extern Expressions_presets Happy;
extern Expressions_presets Sad;
extern Expressions_presets Angry;
extern Expressions_presets Furious;
extern Expressions_presets Bored;
extern Expressions_presets Surprised;
extern Expressions_presets Scared;
extern Expressions_presets Worried;
extern Expressions_presets Sleepy;
extern Expressions_presets Focused;
extern Expressions_presets Alert;
extern Expressions_presets Lost;
extern Expressions_presets Confused;

// 视线控制动画
extern Expressions_presets Look_left;
extern Expressions_presets Look_right;
extern Expressions_presets Look_up;
extern Expressions_presets Look_down;

// 眨眼动画
extern Expressions_presets Wink_left;
extern Expressions_presets Wink_right;

// 复合及状态动画
extern Expressions_presets Charging_start;
extern Expressions_presets Low_battery;
extern Expressions_presets Full_battery;


enum class Expressions_presets_enum : uint8_t
{// 声明所有表情动画对象
    Normal,
    Blink,
    Happy,
    Sad,
    Angry,
    Furious,
    Bored,
    Surprised,
    Scared,
    Worried,
    Sleepy,
    Focused,
    Alert,
    Lost,
    Confused,
    Look_left,
    Look_right,
    Look_up,
    Look_down,
    Wink_left,
    Wink_right,
    Charging_start,
    Low_battery,
    Full_battery
};

#endif
