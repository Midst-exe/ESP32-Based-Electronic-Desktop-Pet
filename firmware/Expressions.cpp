
#include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> // Or #include <Adafruit_SH1106.h> for SH1106 based display
#include "Irisoled.h"
// #include "IrisoledAnimation.h"
#include "Expressions.h"
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
static Emotion currentEmotion = EMO_NONE; // 记录当前播放的表情
static unsigned long lastTime = 0;
static uint8_t frame = 0;

// --- 表情数据配置区 ---

// =======================
// NORMAL / IDLE
// =======================
static const uint16_t delay_normal[] = {1000};
static const unsigned char* frame_normal[] = {Irisoled::normal};

/* =======================
    BLINK（眨眼循环）
=======================*/
static const uint16_t delay_blink[] = {400, 120, 400};
static const unsigned char* frame_blink[] = {
    Irisoled::normal,
    Irisoled::blink,
    Irisoled::normal
};

// =======================
// HAPPY（笑）
// =======================
static const uint16_t delay_happy[] = {300, 300, 500};
static const unsigned char* frame_happy[] = {
    Irisoled::normal,
    Irisoled::happy,
    Irisoled::happy
};

// =======================
// SAD（难过）
// =======================
static const uint16_t delay_sad[] = {500, 500, 800};
static const unsigned char* frame_sad[] = {
    Irisoled::sad,
    Irisoled::normal,
    Irisoled::sad
};

// =======================
// ANGRY（生气）
// =======================
static const uint16_t delay_angry[] = {200, 200, 400};
static const unsigned char* frame_angry[] = {
    Irisoled::angry,
    Irisoled::furious,
    Irisoled::angry
};

// =======================
// FURIOUS（暴怒加强版）
// =======================
static const uint16_t delay_furious[] = {150, 150, 150, 400};
static const unsigned char* frame_furious[] = {
    Irisoled::furious,
    Irisoled::angry,
    Irisoled::furious,
    Irisoled::normal
};

// =======================
// BORED（无聊）
// =======================
static const uint16_t delay_bored[] = {800, 800};
static const unsigned char* frame_bored[] = {
    Irisoled::bored,
    Irisoled::normal
};

    // =======================
    // SURPRISED（惊讶）
    // =======================
    static const uint16_t delay_surprised[] = {200, 600};
    static const unsigned char* frame_surprised[] = {
        Irisoled::surprised,
        Irisoled::normal
    };

    // =======================
    // SCARED（害怕）
    // =======================
    static const uint16_t delay_scared[] = {300, 300, 300};
    static const unsigned char* frame_scared[] = {
        Irisoled::scared,
        Irisoled::surprised,
        Irisoled::scared
    };

    // =======================
    // WORRIED（担心）
    // =======================
    static const uint16_t delay_worried[] = {600, 600};
    static const unsigned char* frame_worried[] = {
        Irisoled::worried,
        Irisoled::normal
    };

    // =======================
    // SLEEPY（困）
    // =======================
    static const uint16_t delay_sleepy[] = {800, 1200, 800};
    static const unsigned char* frame_sleepy[] = {
        Irisoled::sleepy,
        Irisoled::normal,
        Irisoled::sleepy
    };

    // =======================
    // FOCUSED（专注/发呆思考）
    // =======================
    static const uint16_t delay_focused[] = {700, 700};
    static const unsigned char* frame_focused[] = {
        Irisoled::focused,
        Irisoled::look_up
    };

    // =======================
    // ALERT（警觉）
    // =======================
    static const uint16_t delay_alert[] = {200, 200, 200};
    static const unsigned char* frame_alert[] = {
        Irisoled::alert,
        Irisoled::focused,
        Irisoled::alert
    };

    // =======================
    // DESPAIR（绝望/低落）
    // =======================
    static const uint16_t delay_despair[] = {800, 800, 1200};
    static const unsigned char* frame_despair[] = {
        Irisoled::despair,
        Irisoled::sad,
        Irisoled::despair
    };

    // =======================
    // DISORIENTED（迷茫）
    // =======================
    static const uint16_t delay_disoriented[] = {300, 300, 300, 800};
    static const unsigned char* frame_disoriented[] = {
        Irisoled::look_left,
        Irisoled::look_right,
        Irisoled::look_up,
        Irisoled::disoriented
    };

    // ===============================
    //  视线控制动画（Look Direction）
    // 用于控制OLED“眼球/视线方向变化”
    // ===============================

    //  向左看：单帧静态切换，用于状态表达（例如思考/反应）
    static const uint16_t delay_look_left[] = {600};  
    static const unsigned char* frame_look_left[] = {Irisoled::look_left};  
    
    //  向右看：单帧静态视线偏移
    static const uint16_t delay_look_right[] = {600};  
    static const unsigned char* frame_look_right[] = {Irisoled::look_right};  

    //  向上看：常用于“思考 / 回忆 / 想一想”状态
    static const uint16_t delay_look_up[] = {600};  
    static const unsigned char* frame_look_up[] = {Irisoled::look_up};  

    //  向下看：常用于“失落 / 不好意思 / 情绪下降”
    static const uint16_t delay_look_down[] = {600};  
    static const unsigned char* frame_look_down[] = {Irisoled::look_down};  


    /* ===============================
    // 眨眼动画（Wink Animations）
    // 用于增加拟人感（可随机触发）
     ===============================*/

    // 左眼眨眼动画：快速闭眼 -> 恢复正常
    // 常用于“调皮 / 互动反馈 / 语音回应增强”
    static const uint16_t delay_wink_left[] = {200, 200, 400};  
    static const unsigned char* frame_wink_left[] = {
        Irisoled::wink_left,
        Irisoled::normal,
        Irisoled::normal
    };  

    
    // 右眼眨眼动画：与左眨眼对称
    // 可用于随机眨眼，避免机械感
    static const uint16_t delay_wink_right[] = {200, 200, 400};  
    static const unsigned char* frame_wink_right[] = {
        Irisoled::wink_right,
        Irisoled::normal,
        Irisoled::normal
    };  


// 引擎函数：放在 loop 里持续运行
void run_emotion_engine(Adafruit_SSD1306 &display) {
    if (currentEmotion == EMO_NONE) return; // 没事干就歇着

    // 这里根据不同的表情 ID，设置不同的帧序列和时间
    const unsigned char** frames = nullptr;
    const uint16_t* delays = nullptr;
    uint8_t totalFrames = 0;

    // 根据当前 ID 挑选数据
    switch (currentEmotion) {
        case EMO_NORMAL:
            frames = frame_normal; delays = delay_normal; totalFrames = 1;
            break;
        case EMO_BLINK:
            frames = frame_blink; delays = delay_blink; totalFrames = 3;
            break;
        case EMO_HAPPY:
            frames = frame_happy; delays = delay_happy; totalFrames = 3;
            break;
        case EMO_SAD:
            frames = frame_sad; delays = delay_sad; totalFrames = 3;
            break;
        case EMO_ANGRY:
            frames = frame_angry; delays = delay_angry; totalFrames = 3;
            break;
        case EMO_FURIOUS:
            frames = frame_furious; delays = delay_furious; totalFrames = 4;
            break;
        case EMO_BORED:
            frames = frame_bored; delays = delay_bored; totalFrames = 2;
            break;
        case EMO_SURPRISED:
            frames = frame_surprised; delays = delay_surprised; totalFrames = 2;
            break;
        case EMO_SCARED:
            frames = frame_scared; delays = delay_scared; totalFrames = 3;
            break;
        case EMO_WORRIED:
            frames = frame_worried; delays = delay_worried; totalFrames = 2;
            break;
        case EMO_SLEEPY:
            frames = frame_sleepy; delays = delay_sleepy; totalFrames = 3;
            break;
        case EMO_FOCUSED:
            frames = frame_focused; delays = delay_focused; totalFrames = 2;
            break;
        case EMO_ALERT:
            frames = frame_alert; delays = delay_alert; totalFrames = 3;
            break;
        case EMO_DESPAIR:
            frames = frame_despair; delays = delay_despair; totalFrames = 3;
            break;
        case EMO_DISORIENTED:
            frames = frame_disoriented; delays = delay_disoriented; totalFrames = 4;
            break;

        // 视线控制动画
        case EMO_LOOK_LEFT:
            frames = frame_look_left; delays = delay_look_left; totalFrames = 1;
            break;
        case EMO_LOOK_RIGHT:
            frames = frame_look_right; delays = delay_look_right; totalFrames = 1;
            break;
        case EMO_LOOK_UP:
            frames = frame_look_up; delays = delay_look_up; totalFrames = 1;
            break;
        case EMO_LOOK_DOWN:
            frames = frame_look_down; delays = delay_look_down; totalFrames = 1;
            break;

        // 眨眼/单眼互动动画
        case EMO_WINK_LEFT:
            frames = frame_wink_left; delays = delay_wink_left; totalFrames = 3;
            break;
        case EMO_WINK_RIGHT:
            frames = frame_wink_right; delays = delay_wink_right; totalFrames = 3;
            break;

        default:
            currentEmotion = EMO_NONE;
            return;
        }

        // 核心安全检查：如果指针没被赋值，绝对不要往下走
        if (frames == nullptr || delays == nullptr) {
            currentEmotion = EMO_NONE;
            return;
        }

    // 4. 通用的计时与绘图逻辑
    unsigned long now = millis();
    
    // 安全检查：防止 frame 索引越界
    if (frame >= totalFrames) frame = 0; 

    if (now - lastTime >= delays[frame]) {
        lastTime = now;
        frame++;
        if (frame >= totalFrames) {
            // 如果希望循环播放 NORMAL，就设为 NORMAL
            // 如果希望播完就停，设为 NONE
            currentEmotion = EMO_NORMAL; 
            frame = 0; // 重置帧索引
            return;
        }
    }

    display.clearDisplay();
    display.drawBitmap(0, 0, frames[frame], 128, 64, 1);
    display.display();
}

// 触发函数：主程序点单执行
void trigger_emotion(Emotion emo) {
    // 如果请求的表情已经在播了，且还没播完，就不要重置 frame
    if (currentEmotion == emo) return;

    currentEmotion = emo;
    frame = 0;
    lastTime = millis();
}