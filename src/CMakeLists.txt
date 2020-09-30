include(core/CMakeLists.txt)
include(graphics/CMakeLists.txt)
include(modules/CMakeLists.txt)
include(utils/CMakeLists.txt)

add_executable(game-engine-3D
        ${UTILS}
        main.cpp)