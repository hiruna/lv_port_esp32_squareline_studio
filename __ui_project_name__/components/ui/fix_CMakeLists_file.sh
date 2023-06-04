#!/usr/bin/env bash

sed -i 's/add_library.*/idf_component_register(SRCS ${SOURCES} REQUIRES lvgl)/g' CMakeLists.txt