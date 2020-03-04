
set(srcs "time_manager.c" "sntp.c" "time_formatter.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES wifi_manager)
