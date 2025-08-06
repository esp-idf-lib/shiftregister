if(${IDF_VERSION_MAJOR} STREQUAL 5 AND ${IDF_VERSION_MINOR} STRLESS 3)
    set(priv_requires driver/gpio.h)
else()
    set(priv_requires esp_driver_gpio)
endif()

idf_component_register(SRCS "shiftregister_gpio.c"
                    PRIV_REQUIRES ${priv_requires}
                    INCLUDE_DIRS "include")

# include common cmake file for components
set(ESP_IDF_LIB_CMAKE ${CMAKE_CURRENT_LIST_DIR}/common/cmake/esp-idf-lib.cmake)
if(EXISTS ${ESP_IDF_LIB_CMAKE})
    include(${ESP_IDF_LIB_CMAKE})
else()
    message(WARNING "${ESP_IDF_LIB_CMAKE} not found")
endif()
