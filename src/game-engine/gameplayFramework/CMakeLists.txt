include(${CMAKE_CURRENT_LIST_DIR}/components/CMakeLists.txt)

list(APPEND GAMEPLAY_FRAMEWORK
        ${CMAKE_CURRENT_LIST_DIR}/GameObject.hpp
        ${CMAKE_CURRENT_LIST_DIR}/GamePlayFramework.hpp
        ${COMPONENTS})

include_directories(${CMAKE_CURRENT_LIST_DIR})