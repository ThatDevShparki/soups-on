cmake_minimum_required(VERSION 3.27)

set(project "soups-on")
project(${project}
        LANGUAGES CXX
        VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -pedantic-errors")


# install dependencies
include(FetchContent)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# create executable
set(src_files
        src/AssetManager.cpp
        src/Entity.cpp
        src/EntityManager.cpp
        src/Game.cpp
        src/Map.cpp
        src/main.cpp
        src/Vec2.cpp
)
add_executable(${project} ${src_files})

target_link_libraries(${project} PRIVATE sfml-graphics)
target_compile_features(${project} PRIVATE cxx_std_17)

include_directories(include)

install(TARGETS ${project})


# move assets over
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})