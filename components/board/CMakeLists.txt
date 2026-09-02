idf_component_register(SRCS "BSP.cpp" "nvs_Init.cpp" "UART_Init.cpp" "I2C_Init.cpp"
                    INCLUDE_DIRS include
                    REQUIRES 
                        esp_driver_i2c 
                        esp_driver_spi 
                        esp_driver_uart 
                        esp_driver_gpio 
                        nvs_flash        
                    )
