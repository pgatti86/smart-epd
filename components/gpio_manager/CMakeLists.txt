
set(srcs "gpio_manager.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include")
