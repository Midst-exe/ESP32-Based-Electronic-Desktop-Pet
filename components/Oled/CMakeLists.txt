idf_component_register(SRCS "driver/ssd1306_driver.cpp" "framebuffer/ssd1306_framebuffer.cpp" "graphics/ssd1306_graphics.cpp"
                        INCLUDE_DIRS 
                                 "driver"
                                 "framebuffer"
                                 "graphics"   
                        REQUIRES Oled_resource 
                                esp_driver_i2c)
