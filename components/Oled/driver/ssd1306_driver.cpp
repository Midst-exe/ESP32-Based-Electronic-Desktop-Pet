/*  
    oled driver
    主要负责OLED初始化
    命令发送
    数据发送
    整屏刷新
*/

#include "ssd1306_driver.h"
#include "esp_log.h"
#include "string.h"
#include "driver/i2c_master.h"

// 设备驱动层日志 TAG
static const char *TAG = "SSD1306_DRV";


/** 
 * @brief 向 SSD1306 发送命令
 * @param dev OLED 设备句柄
 * @param cmd 命令字节
 * @return ESP_OK 表示成功，其他值表示失败
 */
esp_err_t ssd1306_write_cmd(ssd1306_t *dev, uint8_t cmd) {
    uint8_t write_buf[2] = {0x00, cmd}; //0x00表示截下来传输的是命令，而非动画数据


// esp_err_t i2c_master_transmit(i2c_master_dev_handle_t i2c_dev, const uint8_t *write_buffer, size_t write_size, int xfer_timeout_ms)
// 参数分别是 I2C_设备句柄、写缓冲区、写大小、传输超时时间

    return i2c_master_transmit(dev->i2c_dev, write_buf, sizeof(write_buf), -1);// -1表示无限等待，直到传输完成
}


/** 
 * @brief 向 SSD1306 发送数据
 * @param dev OLED 设备句柄
 * @param data 数据指针
 * @param size 数据大小
 * @return ESP_OK 表示成功，其他值表示失败
 */
esp_err_t ssd1306_write_data(ssd1306_t *dev, const uint8_t *data, size_t size) {
    // 为了极致性能，我们需要把 0x40 前缀和数据打包在一起发送
    // 避免频繁启动 I2C 传输
    uint8_t *write_buf = new uint8_t[size + 1];// +1是为了放置控制字节0x40
    if (write_buf == NULL) {
        return ESP_ERR_NO_MEM;
    }
    write_buf[0] = 0x40; // 0x40 表示后面紧跟的是显存数据


    //void *memcpy(void *dest, const void *src, size_t n);
    // memcpy函数用于将src所指向的内存区域的前n个字节复制到dest所指向的内存区域。
    // 这里我们把数据拷贝到write_buf的第二个字节开始
    memcpy(&write_buf[1], data, size);

    esp_err_t ret = i2c_master_transmit(dev->i2c_dev, write_buf, size + 1, -1);//是否无限等待，直到传输完成
    delete[] write_buf;// 释放动态分配的内存
    write_buf = nullptr; // 良好的习惯：释放后将指针置空
    return ret;//返回传输结果
}

/** 
 * @brief 初始化 SSD1306 OLED 显示器
 * @param dev OLED 设备句柄
 * @param device_address 设备地址
 * @param bus_handle I2C 总线句柄
 * @return ESP_OK 表示成功，其他值表示失败
 */
esp_err_t ssd1306_driver_init(ssd1306_t *dev,uint8_t device_address,i2c_master_bus_handle_t bus_handle) {
    

    ESP_LOGI(TAG,"device_address:%d",device_address);
    if(device_address != OLED_ADDR_3D_96 && device_address != OLED_ADDR_3D_91)  return ESP_FAIL; //地址非法

    dev->oled_addr =  device_address; // 设置结构体显式地址
    // 1. 将设备挂载到已初始化的 I2C 总线上
    i2c_device_config_t dev_cfg = {
        dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7,// 设置7位设备地址
        dev_cfg.device_address = device_address,// 设置设备地址为
        dev_cfg.scl_wait_us = 0,  /*!< Timeout value. (unit: us). Please note this value should not be so small that it can handle stretch/disturbance properly. If 0 is set, that means use the default reg value*/
        dev_cfg.scl_speed_hz = I2C_SCL_400KHz,// 设置 I2C 时钟频率
        dev_cfg.flags.disable_ack_check = false// 启用 ACK 检查
    };
    
    esp_err_t ret = i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev->i2c_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "无法添加 I2C 设备");
        return ret;
    }

    // 2. 设置 SSD1306 官方标准的寄存器初始化配置项【此为0.96寸，0.91会在第3步修改】
    uint8_t init_cmds[] = {
        0xAE, // 关闭屏幕显示 (Display OFF)
        0xD5, 0x80, // 设置时钟分频因子/振荡器频率
        0xA8, 0x3F, // 设置路数 (Multiplex Ratio) 为 64行
        0xD3, 0x00, // 设置显示偏移 (Display Offset) 为 0
        0x40, // 设置显示开始行 (Start Line) 为 0
        0x8D, 0x14, // 开启电荷泵降压（必须开启，否则屏幕不亮！）
        0x20, 0x00, // 设置内存地址模式为：水平地址模式 (Horizontal Addressing Mode)
        0xA1, // 段重映射 (Segment Re-map)：Column 127 映射到 SEG0 (翻转/适配PCB布局)  上下列 反转
        0xC8, // 行扫描方向 (COM Output Scan Direction)：反向 (翻转/适配PCB布局)   左右行 反转
        0xDA, 0x12, // 设置 COM 硬件引脚配置  硬件配置方面
        0x81, 0xCF, // 设置对比度亮度 (Contrast Control) 0x00~0xFF  也就是发光强度，，可配置发光渐强减弱的动画，呼吸灯...，硬件层只能做到全屏刷新，局部的话需要做其他设计
        0xD9, 0xF1, // 设置预充电周期
        0xDB, 0x40, // 设置 VCOMH 电压倍率
        0xA4, // 全屏输出整个显存内容 (Output follows RAM)
        0xA6, // 正常显示模式（不反相）
        0xAF  // 开启屏幕显示 (Display ON)
    };


    // 3. 设置内部缓冲区大小，并重置为0x00
    // 若为0.91寸
    if ( device_address == OLED_ADDR_3D_91 ) {
        dev->buffer.resize(SSD1306_BUFFER_SIZE_512,0x00);
        init_cmds[4] = 0x1F; // 第5项 路数设置成 32 行
        init_cmds[15] = 0x02; // 第16项 设置硬件引脚配置
    }
    else // 否则为0.96寸
        dev->buffer.resize(SSD1306_BUFFER_SIZE_1024,0x00);


    // 4. 依次发送初始化命令
    for (size_t i = 0; i < sizeof(init_cmds); i++) {
        ret = ssd1306_write_cmd(dev, init_cmds[i]);
        if (ret != ESP_OK) return ret;
    }

    ESP_LOGI(TAG, "OLED 驱动层初始化成功！");
    return ESP_OK;
}

esp_err_t ssd1306_update_screen(ssd1306_t *dev) {
    esp_err_t ret;
    
    // 因为初始化设置了水平地址模式 (Horizontal Mode)，
    // 我们只需要把光标重置到左上角 (0,0)，然后一口气把 1024 字节倒进去即可！
    // 64*8=512字节，OLED屏幕是单色的，每个像素点只需要1bit，所以总共需要 512*2=1024 字节 = 1KB


    /* ===============先用命令配置=============== */
    //逐个写入命令，在这不必考量  命令过多导致缓冲区溢出。
    //因为我们只发送了 6 个命令字节，缓冲区大小是 1024 字节，足够容纳这些命令字节。
    //在 I2C 传输中，命令字节和数据字节是分开发送的，所以不会有缓冲区溢出的问题
    //配置对应缓冲区大小在 i2c_master_transmit() 函数中，缓冲区大小是由 write_size 参数决定的，这里传入的是 sizeof(write_buf)，即 2 字节，足够容纳命令字节和数据字节。
    ret = ssd1306_write_cmd(dev, 0x21); // 设置列地址范围命令
    ret = ssd1306_write_cmd(dev, 0x00); // 列起始：0
    ret = ssd1306_write_cmd(dev, 0x7F); // 列结束：127

    ret = ssd1306_write_cmd(dev, 0x22); // 设置页地址范围命令
    ret = ssd1306_write_cmd(dev, 0x00); // 页起始：0

    if ( dev->oled_addr == OLED_ADDR_3D_91 ){
        ret = ssd1306_write_cmd(dev, 0x03); // 页结束：3 (共4页，每页8像素高);
            // 把单片机 RAM 里的 buffer 数据推到屏幕 GDDRAM 中
        ret = ssd1306_write_data(dev, dev->buffer.data(), SSD1306_BUFFER_SIZE_512);
    }
    else{ // 否则为0.96寸
        ret = ssd1306_write_cmd(dev, 0x07); // 页结束：7 (共8页，每页8像素高)
            // 把单片机 RAM 里的 buffer 数据推到屏幕 GDDRAM 中
        ret = ssd1306_write_data(dev, dev->buffer.data(), SSD1306_BUFFER_SIZE_1024);
    }
    /* ===============再传输数据=============== */



    return ret;
}


void ssd1306_clear_buffer(ssd1306_t *dev, uint8_t color) {
    // memset(dev->buffer.data(),color,dev->buffer.size());
    dev->buffer.assign(dev->buffer.size(),color);
}
