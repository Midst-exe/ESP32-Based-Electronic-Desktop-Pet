idf_component_register(SRCS "state_manager.cpp"
                       INCLUDE_DIRS include
                       REQUIRES esp_event
                                Event_Manager
                       )
