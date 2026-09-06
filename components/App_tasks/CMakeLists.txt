idf_component_register(SRCS "wifi_init_task.cpp" "Wifi_Init_task.cpp" "test_task.cpp" "UART_Transmit.cpp" "Expressions_task.cpp" "Wi-fi_Web_manage.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES 
                        Board
                        Expressions
                        Oled
                        Oled_resource
                        Event_Manager
                        State_Manager
                        Wifi
                        WebServer
                    )