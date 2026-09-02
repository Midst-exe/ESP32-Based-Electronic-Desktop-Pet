/**
 * @file wifi.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief wifi连接逻辑和问题处理
 * @version 0.1
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "Wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "webserver.h"
#include "app_events.h"



#define AP_WIFI_SSID "LTF_exe"
#define AP_WIFI_PASSWORD ""
// #define AP_WIFI_PASSWORD "123456789"


#define STA_WIFI_SSID "STA_wifi_ssid"
#define STA_WIFI_PASSWORD "password"


#define ON_AP_PASSWARD 0 // AP无密码模式开关

static uint8_t reconnect_times = 0; // 重连次数重置
#define MAX_RECONNECT_TIMES 3  // 最大重连次数

static const char *TAG = "WiFi_Manager";

/**
 * @brief wifi连接成功handler，在成功连接时返回消息
 * 
 * @param event_handler_arg Oled设备句柄
 * @param event_base 事件头名
 * @param event_id 事件id
 * @param event_data 事件数据 地址
 */
static void wifi_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){
    
    if( event_base == WIFI_EVENT ){
        switch (event_id){
// ************************************** TODO ************************************
/*                方案一实现：
                1. 列出  scan的wifi，并输入密码链接 ，以及未列出的wifi登录
                2. 密码错误返回信息
                3. 连接成功返回信息

                方案二实现：
                直接输入SSID和密码
*/

    // 实现上位机日志打印
        case WIFI_EVENT_STA_CONNECTED: // 连接成功
            ESP_LOGI(TAG,"Wi-Fi Station connected to AP!");
            g_wifi_config_status = WIFI_CONFIG_SUCCESS;
            break;
        case WIFI_EVENT_STA_DISCONNECTED: {// 连接失败 或者 断连
            // 获取断连原因
            wifi_event_sta_disconnected_t* disconnected_data = (wifi_event_sta_disconnected_t*) event_data;
            if(disconnected_data->reason == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT ||   // 四次握手超时
                disconnected_data->reason == WIFI_REASON_AUTH_FAIL ||               // 身份验证失败，密码被拒绝
                disconnected_data->reason == WIFI_REASON_AUTH_EXPIRE                // 身份验证超时
            ) {
                ESP_LOGE("WIFI", "连接失败：密码错误或身份验证失败 (Reason: %d)", disconnected_data->reason);
                g_wifi_config_status = WIFI_CONFIG_PASSWORD_ERROR; // 配网状态：密码错误
            break;}

            else if (disconnected_data->reason ==WIFI_REASON_NO_AP_FOUND) // 找不到 Wi-Fi
            {   ESP_LOGE("WIFI", "连接失败：未找到指定的 WIFI SSID");
                g_wifi_config_status = WIFI_CONFIG_SSID_NO_FOUND; // 配网状态：未找到WiFi
            break;}

            ESP_LOGE("WIFI", "其他断开原因 (Reason: %d)\n尝试重连该WLAN...", disconnected_data->reason);
            // 重连逻辑
            if(reconnect_times >= MAX_RECONNECT_TIMES) {// 判断是否超过重连次数
                ESP_LOGE(TAG,"多次重连失败！");
                g_wifi_config_status = WIFI_CONFIG_FAILED; // 配网状态：重连失败
                break;} 
            ESP_LOGE(TAG,"重连次数%d",reconnect_times+1);

            g_wifi_config_status = WIFI_CONFIG_CONNECTING; // 配网状态：正在连接
            // 尝试重新连接
            reconnect_times++; // 重连次数加1 
            esp_err_t err = esp_wifi_connect();
            if (err != ESP_OK){
                ESP_LOGE(TAG,"重新连接失败: %s",esp_err_to_name(err));
                g_wifi_config_status = WIFI_CONFIG_FAILED; // 配网状态：重连失败
                break;
            }
            break;}
        case WIFI_EVENT_STA_AUTHMODE_CHANGE:{ // wifi改变了认证方式
            ESP_LOGE(TAG,"The auth mode of AP connected by device's station changed!");
            break;}
        default:
            break;
        }
// ************************************** End ************************************    
    }
    // 获取ip
    else if(event_base == IP_EVENT){

        // 可添加功能

        switch (event_id){
        case IP_EVENT_STA_GOT_IP:{
        // 获取ip数据
            ip_event_got_ip_t* netif_data = (ip_event_got_ip_t*) event_data;
            //IPSTR配置好的宏，同时用IP2STR解析，转为点分十进制
            ESP_LOGI(TAG,"ip_addr: " IPSTR,IP2STR(&netif_data->ip_info.ip)); 
            // 修改配网状态：连接成功
            g_wifi_config_status = WIFI_CONFIG_GET_IP;
            reconnect_times = 0; // 修改重连次数
            break;}
        default:
            break;
        }
    }    
}

// wifi连接回调
void wifi_connect_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){

    // 配置 wifi
    wifi_credentials_t* wifi_info = (wifi_credentials_t*) event_data;



    // 配置sta成员,并写入nvs
    // 修改ssid成员
    wifi_config_t sta_config = {};
    snprintf(
        (char *)sta_config.sta.ssid,
        sizeof(sta_config.sta.ssid),
        "%s",
        wifi_info->ssid
    );
    // // 修改password成员
    snprintf(
        (char *)sta_config.sta.password,
        sizeof(sta_config.sta.password),
        "%s",
        wifi_info->password
    );
    // 确保配置后的模式仍是APSTA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    if (g_wifi_config_status == WIFI_CONFIG_CONNECTING) {
        ESP_LOGW("WiFi", "WiFi正在连接，拒绝重复配置!");
        return;
    }    

    // 客户端多次点击连接，会导致重启
    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA,&sta_config);
        
    if (ret != ESP_OK) {
        ESP_LOGE("WiFi_Manager",
                 "esp_wifi_set_config failed: %s",
                 esp_err_to_name(ret));
    }

    // 先断连，在重连
    esp_wifi_disconnect();
    // 修改配网状态
    g_wifi_config_status = WIFI_CONFIG_CONNECTING;

    // 向指定的 Wi-Fi 热点（SSID）发送认证和关联请求
    // 连接成功后抛出 WIFI_EVENT_STA_CONNECTED
    // 获取 IP 后抛出 IP_EVENT_STA_GOT_IP
    // 若失败抛出 WIFI_EVENT_STA_DISCONNECTED
    ESP_ERROR_CHECK(esp_wifi_connect());

}

void wifi_Init(){
    // 连接逻辑， 主要负责AP初始化
      /* 初始化netif网络接口对象
  初始化 ESP32 的底层 TCP/IP 网络协议栈（通常是 LwIP）以及网络抽象接口层 */
    ESP_ERROR_CHECK(esp_netif_init());
    // 创建默认的 Wi-Fi Station 接口
    esp_netif_create_default_wifi_sta();

    // 创建默认的 Wi-Fi AP 接口
    esp_netif_create_default_wifi_ap();

    // 初始化wifi配置，利用默认配置WIFI_INIT_CONFIG_DEFAULT()
    wifi_init_config_t wif_init_config_handle = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wif_init_config_handle));
    // 设置wifi mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    // 注册wifi和ip事件监听  wifi_handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
                            WIFI_EVENT,
                            ESP_EVENT_ANY_ID,
                            &wifi_handler,
                            NULL,
                            NULL
                    )
    );
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
                            IP_EVENT,
                            IP_EVENT_STA_GOT_IP,
                            &wifi_handler,
                            NULL,
                            NULL
                    )
    );
     
    // wifi配置句柄
    wifi_config_t wifi_config = {};

    // 配置AP成员
    // 修改ssid成员
    snprintf(
        (char *)wifi_config.ap.ssid,
        sizeof(wifi_config.ap.ssid),
        "%s",
        AP_WIFI_SSID
    );
    // 修改password成员
    snprintf(
        (char *)wifi_config.ap.password,
        sizeof(wifi_config.ap.password),
        "%s",
        AP_WIFI_PASSWORD
    );
    wifi_config.ap.ssid_len = strlen(AP_WIFI_SSID);
    wifi_config.ap.max_connection = 10;
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;

    if (strlen(AP_WIFI_PASSWORD) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        ESP_LOGI(TAG,"********************** AP 无密码状态  开 **************************");
    }



        /* 采用AP配网，先不配置STAssid和password */
    // 配置sta成员
    // 修改ssid成员
    // snprintf(
    //     (char *)wifi_config.sta.ssid,
    //     sizeof(wifi_config.sta.ssid),
    //     "%s",
    //     STA_WIFI_SSID
    // );
    // // 修改password成员
    // snprintf(
    //     (char *)wifi_config.sta.password,
    //     sizeof(wifi_config.sta.password),
    //     "%s",
    //     STA_WIFI_PASSWORD
    // );

    // 配置网络参数（此步骤会安全地进行 NVS 校验）。
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&wifi_config));
    ESP_LOGI(TAG,"AP已建立: "); 
    ESP_LOGI(TAG,"AP_SSID: %s",wifi_config.ap.ssid); 
    ESP_LOGI(TAG,"AP_PASSWORD: %s",wifi_config.ap.password); 
    
    /* 开启web服务器，使用AP配网逻辑 */
    // 启动wed服务器
    ESP_LOGI(TAG,"请链接热点:%s , 并使用浏览器访问192.168.4.1配网！",wifi_config.ap.ssid); 

    // 注册WIFI_CONFIG_EVENT所有事件，对应回调函数http_server_handler，从而调用webserver_init()
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
                            WIFI_CONFIG_EVENT,
                            ESP_EVENT_ANY_ID,
                            &http_server_handler,
                            NULL,
                            NULL
                    )
    );

    // 注册WIFI_CONFIG_EVENT中事件WIFI_CONFIG_EVENT_PERMITED_CONNECT
    // 对应回调函数wifi_connect_handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
                            WIFI_CONFIG_EVENT,
                            WIFI_CONFIG_EVENT_PERMITED_CONNECT,
                            &wifi_connect_handler,
                            NULL,
                            NULL
                    )
    );

    // 手动触发，通知webserver初始化配网
    esp_event_post(WIFI_CONFIG_EVENT,
                    WIFI_CONFIG_EVENT_WEBSERVER_INIT,
                    NULL,
                    0,
                    portMAX_DELAY);
    // 后续连接逻辑交给web服务器，由其产生事件通知wifi_connnet()

    // 启动 Wi-Fi 驱动、分配底层内存、激活 RF 射频模块
    // 成功后抛出WIFI_EVENT_STA_START事件id
    ESP_ERROR_CHECK(esp_wifi_start());
}


    