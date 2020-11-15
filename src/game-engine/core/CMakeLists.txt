include(${CMAKE_CURRENT_LIST_DIR}/commands/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/triggers/CMakeLists.txt)

list(APPEND CORE
        ${COMMANDS}
        ${TRIGGERS}
        ${CMAKE_CURRENT_LIST_DIR}/Core.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Shell.hpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})