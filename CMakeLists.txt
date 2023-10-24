cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(CSTB_PRE LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(3rdparty/SDL2)
add_subdirectory(src)
