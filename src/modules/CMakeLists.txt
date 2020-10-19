include(${CMAKE_CURRENT_LIST_DIR}/physics/CMakeLists.txt)

list(APPEND MODULES
        ${CMAKE_CURRENT_LIST_DIR}/AModule.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ModuleManager.hpp
        ${CMAKE_CURRENT_LIST_DIR}/GameObjectManipulator.hpp
        ${PHYSICS_MODULE})

include_directories(${CMAKE_CURRENT_LIST_DIR})