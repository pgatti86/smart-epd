
set(srcs "epd_manager.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES waveshare time_manager weather_manager)
