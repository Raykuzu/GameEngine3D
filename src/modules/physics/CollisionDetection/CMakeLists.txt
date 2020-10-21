
list(APPEND COLLISION_DETECTION
        ${CMAKE_CURRENT_LIST_DIR}/SphereCollider.hpp
        ${CMAKE_CURRENT_LIST_DIR}/AABBCollider.hpp
        ${CMAKE_CURRENT_LIST_DIR}/AABBCollider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Intersection.hpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
