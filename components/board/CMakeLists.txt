idf_component_register(SRCS "BSP.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES 
                        esp_driver_i2c 
                        esp_driver_spi 
                        esp_driver_uart 
                        esp_driver_gpio 
                    )
