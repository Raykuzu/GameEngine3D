include(${CMAKE_CURRENT_LIST_DIR}/unix/CMakeLists.txt)

list(APPEND WINDOW
        ${CMAKE_CURRENT_LIST_DIR}/IWindow.hpp
        ${UNIX_WINDOW})

include_directories(${CMAKE_CURRENT_LIST_DIR})