#include <vector>
#include <cstdint>
#include "Irisoled.h"

/*
    这里主要声明动画帧数据
*/
// --- 表情数据配置区 ---

// =======================
// NORMAL / IDLE
// =======================
Expressions_presets Normal{
    .frameCount = 1,
    .delays = {1000},
    .frame = {Irisoled::normal}
};

/* =======================
    BLINK（眨眼循环）
=======================*/
Expressions_presets Blink{
    .frameCount = 5,
    .delays = {400, 120, 100, 120, 400},
    .frame = {
        Irisoled::normal,
        Irisoled::blink,
        Irisoled::normal,
        Irisoled::blink,
        Irisoled::normal
    }
};

// =======================
// HAPPY（笑）
// =======================
Expressions_presets Happy{
    .frameCount = 9,
    .delays = {1500, 35, 10, 40, 40, 1800, 40, 40, 10},
    .frame = {
        Irisoled::normal,
        Irisoled::focused,  
        Irisoled::blink_down, 
        Irisoled::happy, 
        Irisoled::excited, 
        Irisoled::excited,     
        Irisoled::happy,   
        Irisoled::blink_up, 
        Irisoled::normal
    }
};


// =======================
// SAD（难过）
// =======================
Expressions_presets Sad{
    .frameCount = 8,
    .delays = {1000, 35, 150, 200, 3500, 400, 200, 10},
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_down,   // 1. 情绪瞬间低落，眼皮微沉（丝滑过渡帧）
        Irisoled::bored,        // 2. 眼神无精打采（心理缓冲帧）
        Irisoled::sad,          // 3. 正式切换为伤心小弯眼
        Irisoled::despair,      // 4. 情绪跌入谷底，变成极度沮丧/绝望眼
        Irisoled::sad,          // 5. 情绪稍微缓和回普通难过
        Irisoled::bored,        // 6. 叹一口气，退回到无聊发呆
        Irisoled::normal        // 7. 重新振作，恢复日常
    }
};

// =======================
// ANGRY（生气）
// =======================
Expressions_presets Angry{
    .frameCount = 8,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        40,    // 1. 瞬间眼神凝聚收窄 40ms（超高帧率滑过，极丝滑）
        200,   // 2. 维持凝聚眼神 200ms（暴风雨前的平静）
        300,   // 3. 转化到普通生气，眉头皱起，维持 300ms
        4000,  // 4. 彻底爆发！变成大发雷霆的极其愤怒眼神【长定格 4.0 秒，交由 RTOS 调度中断】
        400,   // 5. 怒气稍微收敛，返回普通生气 400ms
        200,   // 6. 逐渐平静，回到凝聚眼神 200ms
        10     // 7. 结束当前循环，返回初始发呆状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::focused,      // 1. 听到挑衅，眼神突然一亮聚焦（丝滑过渡帧）
        Irisoled::focused,      // 2. 锁定目标，准备发飙
        Irisoled::angry,        // 3. 正式切换为普通生气（皱起眉头）
        Irisoled::furious,      // 4. 情绪推向高潮，直接气炸（变成火冒三丈的狂暴/极度愤怒眼）
        Irisoled::angry,        // 5. 情绪稍微缓和回普通生气
        Irisoled::focused,      // 6. 余怒未消，依旧凝视
        Irisoled::normal,       // 7. 彻底恢复日常
    }
};

// =======================
// FURIOUS（暴怒加强版）
// =======================
Expressions_presets Furious{
    .frameCount = 4,
    .delays = {150, 150, 150, 400},
    .frame = {
        Irisoled::furious,
        Irisoled::angry,
        Irisoled::furious,
        Irisoled::normal
    }
};

// =======================
// BORED（无聊）
// =======================
Expressions_presets Bored{
    .frameCount = 9,
    .delays = {
        1200,  // 0. 正常发呆看 1.2 秒
        40,    // 1. 瞬间眼皮下垂 40ms（丝滑过渡）
        2500,  // 2. 维持无精打采的眯眯眼【定格 2.5 秒】
        300,   // 3. 缓缓闭上眼，像是深深叹了一口气 300ms
        600,   // 4. 闭眼小憩 600ms
        40,    // 5. 突然惊醒，眼睛瞬间睁大聚焦 40ms
        300,   // 6. 保持惊醒聚焦状态 300ms
        150,   // 7. 发现还是没什么好玩的，眼神再次垂下去 150ms
        10     // 8. 结束当前循环，返回初始发呆状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_down,   // 1. 眼皮微微向下一沉（过渡帧）
        Irisoled::bored,        // 2. 变成极其无聊、提不起精神的眼神
        Irisoled::sleepy,       // 3. 困意袭来，眼睛半闭
        Irisoled::blink,        // 4. 完全闭眼（模拟长叹一口气或打瞌睡）
        Irisoled::focused,      // 5. 好像听到动静，瞬间惊醒抬头
        Irisoled::focused,      // 6. 定神看了一下
        Irisoled::bored,        // 7. 觉得没意思，又瘫回无聊眼神
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// SURPRISED（惊讶）
// =======================
Expressions_presets Surprised{
    .frameCount = 9,
    .delays = {
        1500,  // 0. 正常发呆状态 1.5 秒
        25,    // 1. 突然听到巨响，眼皮瞬间睁开 25ms
        35,    // 2. 第一次震惊瞪眼 35ms
        35,    // 3. 吓到瞳孔微颤收缩 35ms
        35,    // 4. 彻底吓傻大睁眼 35ms
        3000,  // 5. 陷入极度震惊呆滞状态【大定格 3.0 秒，由 RTOS 调度中断】
        400,   // 6. 缓缓缓过神来，转为担心眼神 400ms
        200,   // 7. 叹一口气放松下来 200ms
        10     // 8. 结束当前循环
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_up,     // 1. 眼皮瞬间向上舒展（极速过渡）
        Irisoled::surprised,    // 2. 首次切入惊讶大眼
        Irisoled::scared,       // 3. 惊恐、瞳孔骤缩（模拟巨震错觉）
        Irisoled::surprised,    // 4. 彻底定格大眼
        Irisoled::surprised,    // 5. 维持极度惊讶状态，定格呆滞
        Irisoled::worried,      // 6. 缓过神，转为担心/疑惑
        Irisoled::bored,        // 7. 恢复到无聊发呆
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// SCARED（害怕）
// =======================
Expressions_presets Scared{
    .frameCount = 9,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        30,    // 1. 突然感知到危险，瞬间瞪大眼 30ms（快速过渡）
        150,   // 2. 维持警惕凝视 150ms（突然愣住）
        30,    // 3. 瞬间被吓到，瞳孔骤缩 30ms
        40,    // 4. 眼神惊慌飘忽 40ms
        30,    // 5. 再次极度惊恐 30ms
        2000,  // 6. 陷入瑟瑟发抖的极度害怕状态【大定格 3.2 秒，由 RTOS 调度随时打断】
        500,   // 7. 危险稍微过去，转为委屈忧虑的眼神 500ms
        10     // 8. 结束当前循环
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_up,     // 1. 眼皮瞬间惊起向上舒展
        Irisoled::alert,        // 2. 警惕地盯着危险源
        Irisoled::scared,       // 3. 首次切入极度惊恐眼（瞳孔变小）
        Irisoled::worried,      // 4. 慌张、担心无助
        Irisoled::scared,       // 5. 彻底吓得定格
        Irisoled::scared,       // 6. 持续维持极度害怕状态，定格散发可怜无助感
        Irisoled::worried,      // 7. 渐渐缓过神，变成委屈/担心
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// WORRIED（担心）
// =======================
Expressions_presets Worried{
    .frameCount = 9,
    .delays = {
        1200,  // 0. 正常发呆看 1.2 秒
        40,    // 1. 听到动静，情绪微沉，眼皮微垂 40ms
        1500,  // 2. 露出担心无助的眼神，定格 1.5 秒
        120,   // 3. 焦虑地往左看一眼 120ms
        100,   // 4. 快速拉回视线，重新陷入担心 100ms
        120,   // 5. 又不安地往右看一眼 120ms
        3000,  // 6. 陷入不知所措的长久忧虑中【大定格 3.0 秒，交由 RTOS 调度随时打断】
        400,   // 7. 叹一口气，眼神稍微放松 400ms
        10     // 8. 结束当前循环
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::bored,        // 1. 情绪开始有些低落（过渡帧）
        Irisoled::worried,      // 2. 转化为担心、忧虑的眼神
        Irisoled::look_left,    // 3. 眼神向左瞟，透着不安
        Irisoled::worried,      // 4. 收回眼神
        Irisoled::look_right,   // 5. 眼神向右瞟，寻找安全感
        Irisoled::worried,      // 6. 持续维持担心的神态，定格发呆
        Irisoled::bored,        // 7. 逐渐缓过神，转为普通发呆
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// SLEEPY（困）
// =======================
Expressions_presets Sleepy{
    .frameCount = 9,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        600,   // 1. 困意袭来，眼皮沉重地耷拉下来 600ms
        800,   // 2. 眼睛快眯成一条缝了 800ms
        400,   // 3. 彻底闭上眼，睡了过去 400ms
        30,    // 4. 突然一惊！眼睛瞬间睁大聚焦 30ms
        500,   // 5. 强撑着精神，定神呆滞看四周 500ms
        150,   // 6. 顶不住了，眼神再次迅速垂下去 150ms
        5000,  // 7. 彻底放弃抵抗，沉沉睡去【巨量定格 5.0 秒，交由 RTOS 调度随时打断】
        10     // 8. 结束当前循环，准备进入下一个睡眠周期
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::bored,        // 1. 无精打采，眼皮开始下垂
        Irisoled::sleepy,       // 2. 变成半开半闭的瞌睡眼
        Irisoled::blink,        // 3. 完全闭眼（开始打盹）
        Irisoled::focused,      // 4. 突然惊醒，眼神瞬间凝聚
        Irisoled::focused,      // 5. 强行睁大眼睛硬撑着
        Irisoled::sleepy,       // 6. 视线再次模糊下垂
        Irisoled::blink,        // 7. 彻底闭眼进入深度睡眠，长久定格
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// FOCUSED（专注/发呆思考）
// =======================
Expressions_presets Focused{
    .frameCount = 9,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        35,    // 1. 快速眨眼下沉 35ms
        120,   // 2. 闭眼 120ms（重新调整焦距）
        40,    // 3. 忽然睁开，眼神聚焦 40ms
        3500,  // 4. 陷入深度思考，眼神死死凝聚【大定格 3.5 秒，由 RTOS 调度随时打断】
        300,   // 5. 脑子里灵光一闪，往右瞟一眼 100ms
        300,   // 6. 往左回瞟一眼 100ms
        2500,  // 7. 重新进入第二轮深度沉思【长定格 2.5 秒，由 RTOS 调度随时打断】
        10     // 8. 结束当前循环，返回初始状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_down,   // 1. 准备眨眼
        Irisoled::blink,        // 2. 闭眼
        Irisoled::blink_up,     // 3. 睁眼聚焦
        Irisoled::focused,      // 4. 进入高能专注/思考状态
        Irisoled::look_right,   // 5. 眼神右瞟（仿佛在检索右侧数据）
        Irisoled::look_left,    // 6. 眼神左瞟（仿佛在核对左侧逻辑）
        Irisoled::focused,      // 7. 继续保持专注思考状态
        Irisoled::normal        // 8. 重置准备
    }
};

// =======================
// ALERT（警觉）
// =======================
Expressions_presets Alert{
    .frameCount = 8,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        60,    // 1. 忽然察觉异样，瞬间睁大眼 25ms（电光石火般的反应）
        200,   // 2. 锁死疑似目标，维持警惕 200ms
        80,    // 3. 瞳孔微缩，深度聚焦 40ms
        80,    // 4. 重新撑大眼眶，全神贯注 40ms
        3000,  // 5. 进入终极戒备状态，死死盯住前方【大定格 4.0 秒，由 RTOS 调度随时打断】
        300,   // 6. 确认没有危险后，眼神稍微放松 300ms
        10     // 7. 结束当前循环，返回初始状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_up,     // 1. 眼皮瞬间上扬（超高速过渡帧）
        Irisoled::alert,        // 2. 切换为警惕瞪眼
        Irisoled::focused,      // 3. 眼神突然一紧，聚焦锁定
        Irisoled::alert,        // 4. 维持高度警备瞪眼
        Irisoled::alert,        // 5. 持续保持警觉神态，死盯目标
        Irisoled::bored,        // 6. 警报解除，化为无聊发呆
        Irisoled::normal        // 7. 重置准备
    }
};

// =======================
// DESPAIR（绝望/低落）
// =======================
Expressions_presets Lost{
    .frameCount = 7,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        45,    // 1. 忽然情绪低落，眼皮瞬间耷拉 45ms（丝滑过渡）
        800,   // 2. 露出无精打采的眼神 800ms
        250,   // 3. 缓缓闭上眼，像是很失落得叹了一口气 250ms
        400,   // 4. 闭眼落寞 400ms
        3800,  // 5. 缓缓睁开，变成无助的失落小弯眼【大定格 3.8 秒，由 RTOS 随时打断】
        10     // 6. 结束当前循环，返回初始状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::blink_down,   // 1. 眼皮微微向下一沉
        Irisoled::bored,        // 2. 变成无聊、提不起精神的眼神
        Irisoled::sleepy,       // 3. 情绪继续下沉，半闭双眼
        Irisoled::blink,        // 4. 完全闭眼（模拟叹气）
        Irisoled::sad,          // 5. 睁开眼变成委屈伤心的失落眼神，长定格
        Irisoled::normal        // 6. 重置准备
    }
};

// =======================
// DISORIENTED（迷茫）
// =======================
Expressions_presets Confused{
    .frameCount = 9,
    .delays = {
        1000,  // 0. 正常发呆看 1 秒
        200,   // 1. 忽然感到一丝不对劲，眼神变迟钝 200ms
        40,    // 2. 猛然一愣，瞳孔放大 40ms（发现异常过渡帧）
        300,   // 3. 疑惑地把眼神往左瞟 300ms
        150,   // 4. 收回眼神，正中间发愣 150ms
        300,   // 5. 又迷茫地把眼神往右瞟 300ms
        4000,  // 6. 彻底放弃思考，陷入灵魂出窍的迷茫状态【大定格 4.0 秒，由 RTOS 调度随时打断】
        300,   // 7. 缓过神来，变成死板的无聊发呆 300ms
        10     // 8. 结束当前循环，返回初始状态
    },
    .frame = {
        Irisoled::normal,       // 0. 正常发呆
        Irisoled::bored,        // 1. 眼神开始涣散
        Irisoled::alert,        // 2. 突然愣住（进入困惑状态的触发点）
        Irisoled::look_left,    // 3. 往左看（是在叫我吗？）
        Irisoled::normal,       // 4. 看回中间（不对，没人啊）
        Irisoled::look_right,   // 5. 往右看（那是啥东西？）
        Irisoled::bored,        // 6. 彻底想不通，眼神化为最空洞的无聊呆滞状，长定格
        Irisoled::bored,        // 7. 维持一下
        Irisoled::normal        // 8. 重置准备
    }
};

// ===============================
//  视线控制动画（Look Direction）
// ===============================

Expressions_presets Look_left{
    .frameCount = 1,
    .delays = {600},
    .frame = {Irisoled::look_left}
};

Expressions_presets Look_right{
    .frameCount = 1,
    .delays = {600},
    .frame = {Irisoled::look_right}
};

Expressions_presets Look_up{
    .frameCount = 1,
    .delays = {600},
    .frame = {Irisoled::look_up}
};

Expressions_presets Look_down{
    .frameCount = 1,
    .delays = {600},
    .frame = {Irisoled::look_down}
};

/* ===============================
// 眨眼动画（Wink Animations）
 ===============================*/

Expressions_presets Wink_left{
    .frameCount = 3,
    .delays = {200, 200, 400},
    .frame = {
        Irisoled::wink_left,
        Irisoled::normal,
        Irisoled::normal
    }
};

Expressions_presets Wink_right{
    .frameCount = 3,
    .delays = {200, 200, 400},
    .frame = {
        Irisoled::wink_right,
        Irisoled::normal,
        Irisoled::normal
    }
};

// =================================================================
// 15. 【充电瞬间】 复合动画 (Charging Start) -> 共 8 帧
// =================================================================
Expressions_presets Charging_start{
    .frameCount = 8,
    .delays = {
        40,    // 0. 插电瞬间，电流涌入，眼睛猛然一亮 40ms（超快响应）
        100,   // 1. 极度惊喜，瞳孔放大 100ms
        35,    // 2. 能量过载，眼皮高频微颤 35ms
        35,    // 3. 持续高频颤动 35ms
        40,    // 4. 转换成弯弯笑脸 40ms
        3500,  // 5. 极度舒服、满足地大笑【大定格 3.5 秒，由 RTOS 随时打断】
        300,   // 6. 趋于平稳，变成普通微笑 300ms
        10     // 7. 结束当前循环
    },
    .frame = {
        Irisoled::focused,      // 0. 瞬间聚焦（像被电了一下）
        Irisoled::surprised,    // 1. 惊喜大睁眼
        Irisoled::alert,        // 2. 能量注入，眼眶高张
        Irisoled::focused,      // 3. 再次收紧（模拟电量波动的动态）
        Irisoled::happy,        // 4. 转为开心
        Irisoled::excited,      // 5. 变成星星眼/大笑眼（吸饱电的快乐，长定格）
        Irisoled::happy,        // 6. 变成普通温和微笑
        Irisoled::normal        // 7. 重置准备
    }
};

// low battery / low power warning
Expressions_presets Low_battery{
    .frameCount = 3,
    .delays = {1000, 500, 1000},
    .frame = {
        Irisoled::battery_low,     
        Irisoled::sleepy,   
        Irisoled::battery_low  
    }
};

// full battery 
Expressions_presets Full_battery{
    .frameCount = 3,
    .delays = {1000, 500, 1000},
    .frame = {
        Irisoled::battery_full,     
        Irisoled::alert,       
        Irisoled::battery_full  
    }
};