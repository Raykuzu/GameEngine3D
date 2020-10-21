include(${CMAKE_CURRENT_LIST_DIR}/logger/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/conf/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/games/CMakeLists.txt)

list(APPEND COMMON
        ${COMMON_LOGGER}
        ${COMMON_CONF}
        ${COMMON_GAMES})