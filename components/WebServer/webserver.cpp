/**
 * @file webserver.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief web服务器相关
 * @version 0.1
 * @date 2026-08-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "webserver.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "event_manager.h"
#include "state_manager.h"
#include "esp_log.h"
/* AP 配网逻辑  AP_pw */
const static char *TAG ="WebServer";

// 路由规则1. client请求登陆页面
// 回调如下：
esp_err_t AP_pw_uri_handler(httpd_req_t* req)
{
    // Recv , Process and Send
    // 设置响应头为 HTML 格式
    httpd_resp_set_type(req, "text/html; charset=utf-8");

    // 失败条件1. 文件资源异常
    if (index_html_size == 0) {
        // Return fail to close session //
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "登陆页面丢失...");
        return ESP_FAIL;
    }

    // 将 HTML 发送给客户端
    esp_err_t ret = httpd_resp_send(req, (const char *)index_html_start, index_html_size);
    
    // 失败条件2. 网络传输失败（如手机中途断开 Wi-Fi 或 Socket 异常）,直接返回ESP_FAIL
    if(ret == ESP_FAIL) return ESP_FAIL;

    // On success
    return ESP_OK;
}


// ---------------------------------- TODO -----------------------------------
                            // 修改为任意json，任意字段摘取
// 处理客户端传回的json数据
/**
 * @brief 解析 WiFi 配置 JSON 字符串
 * @attention 本函数仅有一个ESP_OK出口
 * @param json_str 客户端传来的 JSON 字符串，例如 {"ssid": "MyWiFi", "password": "12345678"}
 * @param out_ssid 接收 ssid 的字符数组缓冲区
 * @param ssid_max_len ssid 缓冲区的最大容量
 * @param out_password 接收 password 的字符数组缓冲区
 * @param pwd_max_len password 缓冲区的最大容量
 * @return esp_err_t 
 */
esp_err_t parse_json_wifi_config(const char *json_str, 
                                 char *out_ssid, size_t ssid_max_len, 
                                 char *out_password, size_t pwd_max_len) 
{
    if (json_str == NULL || out_ssid == NULL || out_password == NULL) {
        return ESP_FAIL;
    }

    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        return ESP_FAIL;
    }

    esp_err_t ret = ESP_FAIL;

    cJSON *ssid_item = cJSON_GetObjectItemCaseSensitive(root, "ssid");
    cJSON *pwd_item = cJSON_GetObjectItemCaseSensitive(root, "password");

    if (cJSON_IsString(ssid_item) && (ssid_item->valuestring != NULL) &&
        cJSON_IsString(pwd_item) && (pwd_item->valuestring != NULL)) 
    {
        // 提取 SSID
        strncpy(out_ssid, ssid_item->valuestring, ssid_max_len - 1);
        out_ssid[ssid_max_len - 1] = '\0';

        // 提取 Password
        strncpy(out_password, pwd_item->valuestring, pwd_max_len - 1);
        out_password[pwd_max_len - 1] = '\0';

        // 校验 SSID 是否非空
        if (strlen(out_ssid) > 0) {
            ret = ESP_OK;
        }
    }

    // 统一出口，释放 cJSON 内存
    cJSON_Delete(root);
    return ret;
}
// ---------------------------------- End -----------------------------------


// 路由规则2. server接收client的POST请求并处理
// 回调如下：
esp_err_t AP_post_uri_handler(httpd_req_t* req)
{
    char rec_buf[256] = {0};
    int ret = 0, received = 0;
    int remaining = req->content_len;

    if (remaining >= sizeof(rec_buf)) {
        ESP_LOGE(TAG, "客户端回传请求内容过长！");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "数据量过大");
        return ESP_FAIL;
    }

    while (remaining > 0) {
        ret = httpd_req_recv(req, rec_buf + received, remaining);
        if (ret <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                httpd_resp_send_408(req);
            }
            return ESP_FAIL;
        }
        received += ret;
        remaining -= ret;
    }
    rec_buf[received] = '\0';

    ESP_LOGI(TAG, "收到配网原始数据: %s", rec_buf);

    // 初始化ssid 和 pwd
    wifi_credentials_t wifi_info ={
        .ssid = {0},
        .password = {0}
    };

    // 解析 JSON
    esp_err_t err = parse_json_wifi_config(rec_buf, wifi_info.ssid, sizeof(wifi_info.ssid), wifi_info.password, sizeof(wifi_info.password));

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "解析 SSID 失败或为空");
        httpd_resp_set_type(req, "application/json");
        // 返回 success: false 让前端捕捉
        const char *fail_resp = "{\"success\":false, \"message\":\"Wi-Fi名称不能为空\"}";
        httpd_resp_send(req, fail_resp, HTTPD_RESP_USE_STRLEN);
        return ESP_OK; // 保持连接
    }

    // 4. 给前端响应标准的 JSON 格式
    httpd_resp_set_type(req, "application/json"); 
    const char *resp_str = "{\"success\":true}";
    esp_err_t send_ret = httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    
    if (send_ret != ESP_OK) {
        return ESP_FAIL;
    }

    // 通知wifi连接
    esp_event_post(WIFI_CONFIG_EVENT,
                   WIFI_CONFIG_EVENT_PERMITED_CONNECT,
                   &wifi_info, 
                   sizeof(wifi_credentials_t),
                   portMAX_DELAY);

    return ESP_OK;
}


// 路由规则3. server处理client的GET请求
esp_err_t AP_get_uri_handler(httpd_req_t* req){

    // 测试
    ESP_LOGI(TAG,"进入AP_get_uri_handler  处理");

    httpd_resp_set_type(req, "application/json");
    const char* status_str = "unknown";



// ************************ TODO **************************
  //  用回调、函数指针、事件通知等方式获取当前配网状态，而不是直接访问全局变量
    wifi_config_status_t current_status =  wifi_manager_get_status(); // 获取当前配网状态

// ************************ End **************************

    switch (current_status)
    {
        case WIFI_CONFIG_STATUS_CONNECTING:  // 正在连接
            status_str = "connecting";
            break;

        case WIFI_CONFIG_STATUS_SUCCESS:  // 连接成功
            status_str = "success";
            break;

        case WIFI_CONFIG_STATUS_PASSWORD_ERROR:  // 密码错误
            status_str = "password_error";
            break;

        case WIFI_CONFIG_STATUS_SSID_NO_FOUND: // 未找到WLAN
            status_str = "ssid_not_found";
            break;

        case WIFI_CONFIG_STATUS_FAILED:  // 配置失败
            status_str = "failed";
            break;

        case WIFI_CONFIG_STATUS_MAX_CONNECT_FAILED:  // 多次重连失败
            status_str = "max_connect_failed";
            break;

        case WIFI_CONFIG_STATUS_GET_IP:  // 获取到ip
            status_str = "got_ip";
            break;

        default:
            status_str = "unknown";
            break;
    }
    // 返回JSON的设置
    char response[128];
    snprintf(
        response,
        sizeof(response),
        "{\"status\":\"%s\"}",  // {"status":"....."}
        status_str
    );
    // 如果获取到IP，则追加ip地址
    if(current_status == WIFI_CONFIG_STATUS_GET_IP)
        snprintf(
        response + strlen(response) - 1,// 起始地址加偏移量
        sizeof(response) - strlen(response), // 全部的地址空间
        ",\"ip\":\"%s\"}",  // 去掉之前的"}    加上  ,ip":"....."
        ipv4_addr
        );

    ESP_LOGI(TAG,"Web_Server  已发送:%s",response);

    return httpd_resp_send(
        req,
        response,
        // 该处参数列表为发送的字节数，HTTPD_RESP_USE_STRLEN宏表示自动计算发送长度【只能用于字符串】
        HTTPD_RESP_USE_STRLEN 
    );

}



void https_server_Init(){

    static httpd_handle_t server_handle = NULL; 
    // 避免重复初始化
    if (server_handle != NULL) {
        ESP_LOGW(TAG, "HTTP Server 已经启动");
        return;
    }

    // 开启webserver服务器
    httpd_config_t httpd_config_handle = HTTPD_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(httpd_start(&server_handle,&httpd_config_handle));


    // 注册登陆页uri，以响应client的GET方法
    // 对应路由规则1 的 URI handler structure  发送登录页
    httpd_uri_t AP_pw_uri = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = AP_pw_uri_handler,
    .user_ctx = NULL
    };
    // Register handler
    ESP_ERROR_CHECK(httpd_register_uri_handler(server_handle, &AP_pw_uri));


    // 对应路由规则2 的 URI handler structure  接收客户端的登录信息JSON: SSID和PASSWORD
    httpd_uri_t AP_post_uri = {
    .uri      = "/wifi",
    .method   = HTTP_POST,
    .handler  = AP_post_uri_handler,
    .user_ctx = NULL
    };
    // 注册 uri_handler
    ESP_ERROR_CHECK(httpd_register_uri_handler(server_handle, &AP_post_uri));

    // 对应路由规则3 的 URI handler structure   发送网络状态
    httpd_uri_t AP_get_uri = {
    .uri      = "/wifi_status",
    .method   = HTTP_GET,
    .handler  = AP_get_uri_handler,
    .user_ctx = NULL
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server_handle, &AP_get_uri));
}

// 接收外部事件通知
void http_server_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){
    if(event_base == WIFI_CONFIG_EVENT){
        switch (event_id)
        {
        case WIFI_CONFIG_EVENT_WEBSERVER_INIT: // 初始化webserver
            https_server_Init();
            break;
        default:
            break;
        }
    }
}


void httpd_server_event_init(){
        // 注册WIFI_CONFIG_EVENT所有事件，对应回调函数http_server_handler，从而调用webserver_init()
    esp_err_t ret = (esp_event_handler_instance_register(
                            WIFI_CONFIG_EVENT,
                            ESP_EVENT_ANY_ID,
                            &http_server_handler,
                            NULL,
                            NULL
                    )
    );
    if(ret != ESP_OK) ESP_LOGE(TAG,"WebServer事件初始动作失败!");
}