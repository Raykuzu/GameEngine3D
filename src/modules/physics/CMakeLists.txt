include(${CMAKE_CURRENT_LIST_DIR}/CollisionDetection/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/Transform/CMakeLists.txt)

list(APPEND PHYSICS_MODULE
        ${CMAKE_CURRENT_LIST_DIR}/PhysicsModule.hpp
        ${TRANSFORM}
        ${COLLISION_DETECTION})

include_directories(${CMAKE_CURRENT_LIST_DIR})
