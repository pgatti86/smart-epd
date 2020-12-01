cmake_minimum_required(VERSION 3.5)

set(srcs "apds_manager.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES apds)