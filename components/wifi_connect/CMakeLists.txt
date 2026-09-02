idf_component_register(SRCS "dpp_enrollee_main.c"   
                    INCLUDE_DIRS include
                    PRIV_REQUIRES esp_wifi 
                                    wpa_supplicant 
                                    nvs_flash
                    REQUIRES
                            qrcode
                    INCLUDE_DIRS ".")
