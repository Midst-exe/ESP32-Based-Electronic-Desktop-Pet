# 编写时一些问题

## void draw_char() //画出一个字符的实现逻辑
**对于void draw_char(uint32_t unicode, int16_t x, int16_t y)中注释的解释：**

实现该部分的双重循环是，遍历字形的行和列。bitmap的存储是按照字节形式的，所以在循环体中，每次内层循环要先定位像素点所在字节，然后利用result位循环计数器确定是像素点所在字节哪一位，最后和所在字节按位与，得到当前像素点的亮灭。



### 第二个参数 uint32_t unicode 测试时不能直接输入汉字
可以这样测试： U'测试文字'；而英文字母可以直接用''包装传输。

还要注意，在c++中，采用UTF-8编码，一个英文字母、空格和英文标点等只占1B；而一个中文或者中文标点则占3B。
同时，注意在末尾还有个 NULL终止符 即 "\0" ，该字节值为0x00.

## void draw_string() 画字符串

unicode万国码：为世界上所有文字唯一编码
UTF-8编码：作为一种编码方式，将unicode存储进计算机

**对于UTF-8**
这是一种变长编码，unicode码每个字节前还要有几个bit的前缀。
**前缀的作用**：为了让计算机能够自同步解析字节流，知道一个字符由几个字节组成，以及每个字节扮演什么角色。

在首个字符和后续字符都有前缀。**具体格式如下：**
| UTF-8长度 | 字节结构 | 前缀特征 | Unicode范围 | 常见字符 | 使用场景 |
|---|---|---|---|---|---|
| 1字节 | `0xxxxxxx` | 开头为 `0` | U+0000 ~ U+007F | 英文字母、数字、符号（A、1、!） | 英文文本、代码、ASCII字符 |
| 2字节 | `110xxxxx 10xxxxxx` | 第1字节开头 `110` | U+0080 ~ U+07FF | 欧洲语言、特殊符号（é、α） | 多语言文本、数学符号 |
| 3字节 | `1110xxxx 10xxxxxx 10xxxxxx` | 第1字节开头 `1110` | U+0800 ~ U+FFFF | 中文、日文、韩文 | 中文显示、网页、OLED字库 |
| 4字节 | `11110xxx 10xxxxxx 10xxxxxx 10xxxxxx` | 第1字节开头 `11110` | U+10000 ~ U+10FFFF | Emoji、生僻字（😀） | 表情显示、扩展字符 |

# 关于测试功能：在主函数中的调用

 **以下为测试示例**
void test_task(void *arg)
{
  	ESP_LOGI(TAG, "Test Task Start");

	// const char* TEST_txt = "Hello World!";
	// const char* TEST_txt_0 = "0123456789";

    // oled_init();

	// OLED设备句柄
	// 创建 ssd1306_t 类型句柄 与 设备物理地址绑定、配置初始化等
	ssd1306_t Oled_0;
	ssd1306_init(&Oled_0,OLED_ADDR_0,bus_handle);

		ssd1306_graphics_clear(&Oled_0, 0xff);
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));

    while (1)
    {
  	    ESP_LOGI(TAG, "*********************** 绘制动画 ***********************");
  	    ESP_LOGI(TAG, "*********************** X形 ***********************");
		ssd1306_draw_line(&Oled_0,0,0,127,63,0xff);
		ssd1306_draw_line(&Oled_0,0,63,127,0,0xff);
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));

  	    ESP_LOGI(TAG, "*********************** 正方形 ***********************");
        ssd1306_draw_rect(&Oled_0,53,21,22,22,false,0xff); //正方形，边22，不填充
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));

  	    ESP_LOGI(TAG, "*********************** 圆形 ***********************");
        ssd1306_draw_circle(&Oled_0,63,31,16,false,0xff);
        ssd1306_flush(&Oled_0); // 推送上屏
        vTaskDelay(pdMS_TO_TICKS(3000));
  	    ESP_LOGI(TAG, "*********************** 清除 ***********************");
    	  ssd1306_graphics_clear(&Oled_0, 0x00);
        vTaskDelay(pdMS_TO_TICKS(1000));
  	    ESP_LOGI(TAG, "#################### 绘制动画结束 ####################");

        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 别忘了让出CPU

    }
}
