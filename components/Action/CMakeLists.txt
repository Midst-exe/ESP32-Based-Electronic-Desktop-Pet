idf_component_register(SRCS "Action.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES 
                        Board 
                        esp_driver_gpio
                    )