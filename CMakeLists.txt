cmake_minimum_required(VERSION 3.5)

project(Game-engine-3D)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

if (${UNIX})
    set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -std=c++17 -g3")
endif()
if (${WIN32})
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)
    add_definitions(/D NOMINMAX)
    add_definitions(/D WIN32_LEAN_AND_MEAN)
    set(GUI_TYPE ${WIN32})
endif()

add_subdirectory(src)