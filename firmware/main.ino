/*
  ESP32S3 I2S Microphone -> WebSocket Binary Stream
  Hardware:
    ESP32S3
    INMP441 I2S Microphone
    L298N
    SSD 1306 0.96 OLED

  INMP441 Microphone Pin Connection:
    SCK  -> GPIO_4
    WS   -> GPIO_5
    SD   -> GPIO_6

  L298N Pin Connection:
   IN1   -> GPIO_15
   IN2   -> GPIO_16
   IN3   -> GPIO_17
   IN4   -> GPIO_18

  SSD_1306 OLED Pin Connection: 0.96寸 表情显示用
   SCL   -> GPIO_1 
   SDA   -> GPIO_2 

  SSD_1306 OLED Pin Connection: 0.91寸 串口打印用  【未设置】
   SCL   -> GPIO_11 
   SDA   -> GPIO_12 

*/

#include <WiFi.h>
#include <WebSocketsClient.h>
#include "driver/i2s.h"
#include <ArduinoJson.h>
#include "NetworkManager.h"
#include "Expressions.h"
#include "Action.h"
#include "Hardware_Pin.h"


/* ---------------- WiFi CONFIG ---------------- */
const char* WIFI_SSID = "your_WIFI_SSID";
const char* WIFI_PASS = "your_WIFI_PASS";


/* --------------- WebSocket SERVER ------------ */
const char* WS_HOST = "192.168.251.126";   // 本地服务器  PC_IP
const uint16_t WS_PORT = 8765;
const char* WS_PATH = "/";

/* --------------- AUDIO SETTINGS -------------- */
#define SAMPLE_RATE     16000
#define SAMPLE_BITS     I2S_BITS_PER_SAMPLE_16BIT
#define CHANNEL_FORMAT  I2S_CHANNEL_FMT_ONLY_LEFT

#define AUDIO_PACKET_SIZE 1024   // 每次发送1024字节

/* I2S数据缓冲区 */
uint8_t audioBuffer[AUDIO_PACKET_SIZE];

/*录音标识*/
bool isListening = false;

/*唤醒标识*/
bool isWokenUp = false; 

/* OLED SSD1306  */
// 初始化一个名为 display 的 OLED 屏幕对象
// 参数含义：屏幕宽128, 高64, 使用I2C通信协议(&Wire), 不使用硬件复位引脚(-1)
Adafruit_SSD1306 display(128, 64, &Wire, -1); 

/* =========================================================
   I2S 初始化
   ========================================================= */
void initI2SMic()
{
  /*
    I2S 配置说明：

    SAMPLE_RATE = 16000 Hz
    BITS = 16bit
    MONO = 单声道

    INMP441 使用 Philips I2S 标准
  */

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),

    // 采样率
    .sample_rate = SAMPLE_RATE,

    // 位深
    .bits_per_sample = SAMPLE_BITS,

    // 单声道
    .channel_format = CHANNEL_FORMAT,

    // Philips I2S 标准
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,

    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,

    .dma_buf_count = 8,
    .dma_buf_len = 512,

    .use_apll = false,
    .tx_desc_auto_clear = true, // 播放完自动清除缓存，防止噪音
    .fixed_mclk = 0
  };

  /*
    I2S 引脚定义
  */
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_DOUT, // 对应 GPIO 10，输出给喇叭
    .data_in_num = I2S_SD     // 对应 GPIO 8，从麦克风输入
  };

  // 安装 I2S 驱动
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

  // 设置引脚
  i2s_set_pin(I2S_NUM_0, &pin_config);

  // 清空DMA缓存
  i2s_zero_dma_buffer(I2S_NUM_0);

  Serial.println("I2S Microphone initialized");
}


/* =========================================================
   SETUP
   ========================================================= */
void setup()
{
  Serial.begin(115200);

  /* 初始化 ESP32S3 -> OLED Pin */
  Wire.begin(OLED_SDA, OLED_SCL); 
    
  /* 初始化 OLED 屏幕硬件。
   SSD1306_SWITCHCAPVCC: 指示屏幕电源由内部电路生成。
   0x3C: OLED 屏幕在 I2C 总线上的默认地址（有些屏幕可能是 0x3D）
  */
    // *** 启动屏幕硬件 ***//
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println("SSD1306 allocation failed");
      for(;;); 
  }

  // --- 立即清空 Adafruit 标志 ---
  display.clearDisplay(); 
  display.display();

  // //测试开机动画
  // /*  display.drawCircle(x, y, 半径r, 颜色)
  //     其中颜色：在单色屏中 WHITE = 点亮像素  BLACK = 擦除/不显示
  // */
  // display.drawCircle(64, 32, 30, WHITE);
  // display.drawCircle(64, 32, 29, WHITE);
  // display.drawCircle(64, 32, 28, WHITE);
  // display.drawCircle(64, 32, 27, WHITE);
 
 // 链接wifi个人热点
  init_Network();


  /* 初始化 WebSocket */
  webSocket.begin(WS_HOST, WS_PORT, WS_PATH);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);

  /* 初始化 I2S 麦克风 */
  initI2SMic();

  /*boot按键使能， 上拉*/
  pinMode(BOOT_BUTTON, INPUT_PULLUP);

  /*动作引脚使能， 推挽输出*/
  pinMode(motor_IN1, OUTPUT); 
  pinMode(motor_IN2, OUTPUT); 
  pinMode(motor_IN3, OUTPUT); 
  pinMode(motor_IN4, OUTPUT); 

  //切回表情，开机先眨个眼
  display.clearDisplay();
  display.display();               // 清空文字，准备画脸
  trigger_emotion(EMO_BLINK);     // 触发正常表情（或眨眼）
  
  Serial.println("WiFi已连接，表情引擎启动");


}



/* =========================================================
   LOOP
   ========================================================= */
void loop() {
  //启动表情引擎：负责刷屏幕帧
    run_emotion_engine(display);

  //启动动作任务：负责监控电机时间并在到期时停止
    update_motor_task();

  //启动网络任务：负责处理 WebSocket
    webSocket.loop();

  /* WiFi 重连 */
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Lost, reconnecting...");
    init_Network();
    return; // 跳过本次循环等待连接
  }

  // --- 检测按键状态 ---
  
  // 判断逻辑：BOOT 键按下时为 LOW
  if (digitalRead(BOOT_BUTTON) == LOW) {
    
    // 如果是刚按下去的那一刻
    if (!isListening) {
      Serial.println("\n[录音] 开始...");
      isListening = true;
    }

    size_t bytesRead = 0;
    // 从 I2S 读取音频
    i2s_read(I2S_NUM_0, audioBuffer, AUDIO_PACKET_SIZE, &bytesRead, portMAX_DELAY);

    // 只有在按下期间才发送二进制数据
    if (webSocket.isConnected() && bytesRead > 0) {
      webSocket.sendBIN(audioBuffer, bytesRead);
      // Serial.print("."); // 调试用：打印点代表正在传输
    }
  } 
  
  // 如果按键没有被按下（处于 HIGH 状态）
  else {
    // 检查刚才是不是正在录音？如果是，说明现在是“刚松开”的一刻
    if (isListening) {
      Serial.println("\n[录音] 停止，通知服务器处理...");
      
      // 发送结束指令
      if (webSocket.isConnected()) {
        webSocket.sendTXT("{\"cmd\": \"stop_record\"}");
      }
      
      isListening = false; // 重置状态，等待下次按下
    }
    
    // 没按键时稍微延迟一下，防止 CPU 空转过快
    delay(10); 
  }
}
