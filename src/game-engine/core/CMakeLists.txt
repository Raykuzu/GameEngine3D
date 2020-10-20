include(${CMAKE_CURRENT_LIST_DIR}/commands/CMakeLists.txt)

list(APPEND CORE
        ${COMMANDS}
        ${CMAKE_CURRENT_LIST_DIR}/Core.hpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})