include(${CMAKE_CURRENT_LIST_DIR}/physics/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/graphic/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/trigger/CMakeLists.txt)

list(APPEND MODULES
        ${CMAKE_CURRENT_LIST_DIR}/AModule.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ModuleManager.hpp
        ${PHYSICS_MODULE}
        ${GRAPHIC_MODULE}
        ${TRIGGER_MODULE})

include_directories(${CMAKE_CURRENT_LIST_DIR})