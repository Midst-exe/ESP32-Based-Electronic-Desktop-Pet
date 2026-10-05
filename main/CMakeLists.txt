idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES 
                        App_tasks
                        Board
                        Wifi
                        State_Manager
                        Event_Manager
                    )
