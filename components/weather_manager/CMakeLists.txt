
set(srcs "weather_manager.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_client json storage_manager)
