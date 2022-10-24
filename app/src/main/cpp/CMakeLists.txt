cmake_minimum_required(VERSION 3.18.1)
project("msopengles3")

file(GLOB all_files *.c *.cpp sample/*.cpp ms_opengles/*.cpp)


SET(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libglm)
SET(STBIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libstbImage)
SET(CPPDIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libFFmpeg)


INCLUDE_DIRECTORIES(${GLM_DIR}/include)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/sample)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ms_opengles)


INCLUDE_DIRECTORIES(${STBIMG_DIR}/include)

INCLUDE_DIRECTORIES(${FFMPEG_DIR}/${ANDROID_ABI}/include)
LINK_DIRECTORIES(${FFMPEG_DIR}/${ANDROID_ABI}/lib)  # 链接静态库


add_library(
        msopengles3
        SHARED
        ${all_files})


SET(FFMPEG_LIB avformat avcodec avdevice avfilter avutil swresample swscale fdk-aac x264 z c m dl)

SET(ANDROID_SYSLIBS android log GLESv3 OpenSLES)

target_link_libraries(
        msopengles3
        ${FFMPEG_LIB}
        ${ANDROID_SYSLIBS}
       )