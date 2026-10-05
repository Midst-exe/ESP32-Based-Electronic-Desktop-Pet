idf_component_register(SRCS "Wi-fi_init_task.cpp" "test_task.cpp" "UART_Transmit.cpp" "Expressions_task.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES 
                        Board
                        Expressions
                        Oled
                        Oled_resource
                        Wifi
                        WebServer
                        State_Manager
                        Event_Manager
                    )