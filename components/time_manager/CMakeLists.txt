
set(srcs "time_manager.c" "time_formatter.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include")
