cmake_minimum_required(VERSION 3.27)

set(project "soups-on")
project(${project}
        LANGUAGES CXX
        VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -pedantic-errors")

set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)

set(SFML_BUILD_NETWORK OFF)

set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_WINDOW ON)

# install dependencies
#include(FetchContent)
#
#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
#
#FetchContent_Declare(SFML
#        GIT_REPOSITORY https://github.com/SFML/SFML.git
#        GIT_TAG 2.6.x)
#FetchContent_MakeAvailable(SFML)
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

# create executable
set(src_files
        src/ActionManager.cpp
        src/Animation.cpp
        src/AssetManager.cpp
        src/Entity.cpp
        src/EntityManager.cpp
        src/Game.cpp
        src/Map.cpp
        src/main.cpp
        src/Scene.cpp
        src/SceneMenu.cpp
        src/SceneLevel.cpp
        src/Vec2.cpp
)
add_executable(${project} ${src_files})

target_link_libraries(
        ${project} PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio
)
target_compile_features(${project} PRIVATE cxx_std_17)

include_directories(include)

install(TARGETS ${project})


# move assets over
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})