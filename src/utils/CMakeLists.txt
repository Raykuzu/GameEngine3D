include(${CMAKE_CURRENT_LIST_DIR}/format/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/logger/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/CMakeLists.txt)

list(APPEND UTILS
        ${FORMAT})
list(APPEND UTILS
        ${LOGGER})
list(APPEND UTILS
        ${SERIALIZE})