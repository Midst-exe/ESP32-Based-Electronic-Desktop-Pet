#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <Adafruit_SSD1306.h>

/*-------------------------------------------
    表情编号的含义如下：
        0: EMO_NONE,         // 无动作
        1: EMO_NORMAL,       // 正常状态
        2: EMO_BLINK,        // 眨眼
        3: EMO_HAPPY,        // 开心
        4: EMO_SAD,          // 难过
        5: EMO_ANGRY,        // 生气
        6: EMO_FURIOUS,      // 暴怒
        7: EMO_BORED,        // 无聊
        8: EMO_SURPRISED,    // 惊讶
        9: EMO_SCARED,       // 害怕
        10: EMO_WORRIED,     // 担心
        11: EMO_SLEEPY,      // 困倦
        12: EMO_FOCUSED,     // 专注 / 思考
        13: EMO_ALERT,       // 警觉
        14: EMO_DESPAIR,     // 绝望 / 情绪低落
        15: EMO_DISORIENTED, // 迷茫 / 不知所措
        16: EMO_LOOK_LEFT,   // 向左看
        17: EMO_LOOK_RIGHT,  // 向右看
        18: EMO_LOOK_UP,     // 向上看
        19: EMO_LOOK_DOWN,   // 向下看
        20: EMO_WINK_LEFT,   // 左眼眨眼
        21: EMO_WINK_RIGHT   // 右眼眨眼
---------------------------------------------
*/


// ======================================================
// 1. 定义表情枚举 ID (Emotion)
// 必须与 .cpp 中的 switch case 对应
// ======================================================
enum Emotion {
    EMO_NONE,         // 无动作
    EMO_NORMAL,       // 正常状态
    EMO_BLINK,        // 眨眼
    EMO_HAPPY,        // 开心
    EMO_SAD,          // 难过
    EMO_ANGRY,        // 生气
    EMO_FURIOUS,      // 暴怒
    EMO_BORED,        // 无聊
    EMO_SURPRISED,    // 惊讶
    EMO_SCARED,       // 害怕
    EMO_WORRIED,      // 担心
    EMO_SLEEPY,       // 困倦
    EMO_FOCUSED,      // 专注
    EMO_ALERT,        // 警觉
    EMO_DESPAIR,      // 绝望
    EMO_DISORIENTED,  // 迷茫
    EMO_LOOK_LEFT,    // 向左看
    EMO_LOOK_RIGHT,   // 向右看
    EMO_LOOK_UP,      // 向上看
    EMO_LOOK_DOWN,    // 向下看
    EMO_WINK_LEFT,    // 左眼眨眼
    EMO_WINK_RIGHT    // 右眼眨眼
};

// ======================================================
// 2. 函数声明（供外部调用）
// ======================================================

/**
 * 引擎渲染函数：必须放在主循环 loop() 中持续运行。
 * @param display 传入 OLED 对象引用
 */
void run_emotion_engine(Adafruit_SSD1306 &display);

/**
 * 触发表情函数：在接收到指令（如 JSON）时调用一次。
 * @param emo 想要播放的表情 ID
 */
void trigger_emotion(Emotion emo);

#endif