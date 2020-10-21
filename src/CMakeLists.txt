include(${CMAKE_CURRENT_LIST_DIR}/common/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/game-engine/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/conf/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/games/CMakeLists.txt)

add_executable(game-engine-3D
        ${COMMON}
        ${GAME-ENGINE})

add_library(configuration SHARED
        ${COMMON_LOGGER}
        ${COMMON_CONF}
        ${CONF})

add_library(example1-game SHARED
        ${COMMON_LOGGER}
        ${COMMON_GAMES}
        ${EXAMPLE1-GAME})

if (${UNIX})
    target_link_libraries(game-engine-3D dl)
    target_link_libraries(game-engine-3D pthread)
endif()