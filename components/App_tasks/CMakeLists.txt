idf_component_register(SRCS "test_task.cpp" "UART_Transmit.cpp" "Expressions_task.cpp" "Wi-fi_Web_manage.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES 
                        board
                        Expressions
                        Oled
                        Oled_resource
r