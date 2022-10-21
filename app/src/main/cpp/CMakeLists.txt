cmake_minimum_required(VERSION 3.18.1)
project("msopengles3")

SET(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libglm)
SET(STBIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libstbImage)
SET(CPPDIR ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${GLM_DIR}/include)
INCLUDE_DIRECTORIES(${STBIMG_DIR}/include)

file(GLOB all_files *.c *.cpp)

add_library(
        msopengles3
        SHARED
        ${all_files})


find_library(
        log-lib

        log)


target_link_libraries(
        msopengles3
        -lGLESv3
        -landroid
        ${log-lib})