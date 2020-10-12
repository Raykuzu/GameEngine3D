include(core/CMakeLists.txt)
include(graphics/CMakeLists.txt)
include(modules/CMakeLists.txt)
include(utils/CMakeLists.txt)
include(gameplayFramework/CMakeLists.txt)

add_executable(game-engine-3D
        ${UTILS}
        ${GAMEPLAY_FRAMEWORK}
        ${MODULES}
        main.cpp)