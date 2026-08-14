# I2C初始化
1. **核心逻辑链**

*初始化I2C总线逻辑*
```markdown
    i2c_master_bus_config_t
        |  描述我要什么样的I2C
        ↓
    i2c_new_master_bus()
        |  创建一个I2C总线对象
        ↓
    i2c_master_bus_handle_t
        |  代表这条I2C总线
        ↓
    i2c_master_bus_add_device()
        |  在这条总线上添加OLED
        ↓
    i2c_master_dev_handle_t
        |  代表OLED设备
    i2c_master_transmit()
        ↓
    OLED收到数据
```
2. **核心对象对照**
| 对象 | 作用 |
|------|------|
| `i2c_master_bus_config_t` | 总线配置 |
| `i2c_new_master_bus()` | 创建总线 |
| `bus_handle` | 总线句柄 |
| `i2c_device_config_t` | 设备配置 |
| `i2c_master_bus_add_device()` | 添加设备 |
| `oled_handle` | 设备句柄 |
| `i2c_master_transmit()` | 发送数据 |

# UART初始化

