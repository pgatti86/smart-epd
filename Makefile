#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := smart-epd
EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/epd/components/ $(PROJECT_PATH)/apds9960-idf/components/

include $(IDF_PATH)/make/project.mk



