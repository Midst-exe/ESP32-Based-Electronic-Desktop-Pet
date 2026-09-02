/**
 * @file nvs_init.cpp
 * @author Midst.exe (Midst.exe@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-08-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "BSP.h"

#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

static const char* TAG = "nvs_init" ;


// nvs初始化
esp_err_t nvs_Init(){

    esp_err_t ret = nvs_flash_init();

    if( ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND ){
        ESP_LOGI(TAG,"无NVS空页或版本太旧，尝试：擦除后重新初始化!");
        // 擦除原有块
        ret = nvs_flash_erase();
        if(ret != ESP_OK) ESP_LOGE(TAG,"擦除失败！");
        else{// 重新初始化
            ret = nvs_flash_init();
        }
    }

    else if( ret ==  ESP_ERR_NO_MEM ) ESP_LOGE(TAG,"无法为 NVS 驱动分配足够的内部结构体空间");
    else if (ret == ESP_ERR_NOT_FOUND ) ESP_LOGE(TAG,"未找到NVS分区标记，请修改分区表!");


    if( ret == ESP_OK) {
        ESP_LOGI(TAG,"NVS Initialization successful!");
        return ret;
    }
    else{
        ESP_LOGE(TAG,"NVS Initialization failed!");
        return ret;
    }
}