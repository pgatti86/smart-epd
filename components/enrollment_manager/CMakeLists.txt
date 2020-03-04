
set(srcs "enrollment_manager.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES wifi_manager esp_http_server json utils storage_manager)
