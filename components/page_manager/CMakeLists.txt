cmake_minimum_required(VERSION 3.5)

set(srcs "page_manager.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES waveshare time_manager weather_manager 
                    enrollment_manager dht_manager apds_manager)