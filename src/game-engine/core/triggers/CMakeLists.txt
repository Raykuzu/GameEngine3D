list(APPEND TRIGGERS
        ${CMAKE_CURRENT_LIST_DIR}/ITrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ATrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/AAutoTrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ABiTrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/InputTrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/CollideTrigger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/SceneChange.hpp
        ${CMAKE_CURRENT_LIST_DIR}/SceneTrigger.hpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})