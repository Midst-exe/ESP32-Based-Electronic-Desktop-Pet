**这里主要存储和声明bitmap和表情组帧数据**

**所有bitmap都修改为 SSD1306的Page格式存储**

其中
- Irisoled.cpp：存储每帧bitmap

- Expressions_presets.cpp：存储表情组帧数据。主要包括帧延迟时间、帧数和帧数组等

由Irisoled.h向外声明bitmap像素图数据和动画表情数据
