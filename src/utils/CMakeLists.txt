include(${CMAKE_CURRENT_LIST_DIR}/format/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/logger/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/conf/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/uuid/CMakeLists.txt)

list(APPEND UTILS
        ${LOGGER})
list(APPEND UTILS
        ${FORMAT})
list(APPEND UTILS
        ${SERIALIZE})
list(APPEND UTILS
        ${CONF})
list(APPEND UTILS
        ${UUID})
