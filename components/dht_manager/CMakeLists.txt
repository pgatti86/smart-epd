cmake_minimum_required(VERSION 3.5)

set(srcs "dht_manager.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include")