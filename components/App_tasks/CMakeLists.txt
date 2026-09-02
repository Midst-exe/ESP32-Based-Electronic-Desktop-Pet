idf_component_register(SRCS "test_task.cpp" "UART_Transmit.cpp" "Expressions_task.cpp" "Wi-fi_Web_manage.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES 
                        Board
                        Expressions
                        Oled
                        Oled_resource
                    )