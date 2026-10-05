
idf_component_register(SRCS "state_manager.cpp"
                       INCLUDE_DIRS include
                       REQUIRES "Event_Manager"
                                "esp_event"
                                )
