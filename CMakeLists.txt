# CMake build for Linux/macOS/VS Code. Visual Studio users can keep using PingPong.sln:
# Visual Studio ignores this file unless you open the folder with File > Open > Folder.
cmake_minimum_required(VERSION 3.16)
project(PingPong CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)   # breakpoints work out of the box
endif()

# SFML 2.5+ (Ubuntu: sudo apt install libsfml-dev)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(PingPong
  PingPong/PingPong.cpp
  PingPong/ball.cpp
  PingPong/paddle.cpp
)
target_link_libraries(PingPong PRIVATE sfml-graphics sfml-window sfml-system)

# The game loads "Font/Candal.ttf" relative to the folder it runs from (build/),
# so link build/Font back to the repo's PingPong/Font.
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/PingPong/Font ${CMAKE_BINARY_DIR}/Font SYMBOLIC COPY_ON_ERROR)
