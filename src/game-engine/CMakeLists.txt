include(${CMAKE_CURRENT_LIST_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/graphics/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/modules/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/gameplayFramework/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/window/CMakeLists.txt)

list(APPEND GAME-ENGINE
        ${UTILS}
        ${GAMEPLAY_FRAMEWORK}
        ${MODULES}
        ${CORE}
        ${WINDOW}
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp)