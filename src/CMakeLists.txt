include(core/CMakeLists.txt)
include(graphics/CMakeLists.txt)
include(modules/CMakeLists.txt)
include(utils/CMakeLists.txt)
include(gameplayFramework/CMakeLists.txt)
include(window/CMakeLists.txt)

add_executable(game-engine-3D
        ${UTILS}
        ${GAMEPLAY_FRAMEWORK}
        ${MODULES}
        ${WINDOW}
        main.cpp)

if (${UNIX})
    target_link_libraries(game-engine-3D -lX11)
endif()