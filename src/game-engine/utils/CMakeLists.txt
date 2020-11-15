include(${CMAKE_CURRENT_LIST_DIR}/threads/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/format/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/math/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/uuid/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/dll/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/stb-master/CMakeLists.txt)

list(APPEND UTILS
        ${THREADS})
list(APPEND UTILS
        ${FORMAT})
list(APPEND UTILS
        ${SERIALIZE})
list(APPEND UTILS
        ${MATH})
list(APPEND UTILS
        ${UUID})
list(APPEND UTILS
        ${DLL})