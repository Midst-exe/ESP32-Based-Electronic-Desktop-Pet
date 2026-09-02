idf_component_register(SRCS "wifi_connect.cpp"
                       INCLUDE_DIRS include
                       REQUIRES esp_wifi
                                esp_netif
                                WebServer
                                App_events
                       )
