idf_component_register(SRCS "webserver.cpp"
                    INCLUDE_DIRS include
                    REQUIRES 
                            esp_https_server
                            Event_Manager
                            State_Manager
                    EMBED_FILES
                            "web_resource/index.html"
                        #     "web_resource/Browser_Console.html"
                    )
