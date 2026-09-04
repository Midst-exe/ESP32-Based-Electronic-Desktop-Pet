
idf_component_register(SRCS "wifi_connect.cpp" "wifi_state.cpp"
                       INCLUDE_DIRS "include"
                       PRIV_INCLUDE_DIRS "private"
                       REQUIRES esp_wifi
                              esp_event
                             State_Manager
                             Event_Manager)
