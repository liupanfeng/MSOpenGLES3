cmake_minimum_required(VERSION 3.18.1)
project("msopengles3")

file(GLOB all_files *.c *.cpp sample/*.cpp ms_opengles/*.cpp ms_video_player/*.cpp ms_video_player/*.c)


SET(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libglm)
SET(STBIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libstbImage)
SET(CPPDIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libFFmpeg)

#人脸标定
SET(FACETRACKER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libfacetracker)
INCLUDE_DIRECTORIES(${FACETRACKER_DIR}/include)
LINK_DIRECTORIES(${FACETRACKER_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI})
SET(LIBFACETRACKER facetracker)

#OpenCV
SET(LIBOPENCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OpenCV-android-sdk)
INCLUDE_DIRECTORIES(${LIBOPENCV_DIR}/sdk/native/jni/include)


INCLUDE_DIRECTORIES(${GLM_DIR}/include)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/sample)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ms_opengles)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ms_video_player)

#OpenCV 头文件
INCLUDE_DIRECTORIES(opencv)


INCLUDE_DIRECTORIES(${STBIMG_DIR}/include)

INCLUDE_DIRECTORIES(${FFMPEG_DIR}/${CMAKE_ANDROID_ARCH_ABI}/include)
LINK_DIRECTORIES(${FFMPEG_DIR}/${CMAKE_ANDROID_ARCH_ABI}/lib)  # 链接静态库

##将OpenCV 编译成动态库 然后进行链接
add_library(LIBOPENCV SHARED IMPORTED)
set_target_properties(LIBOPENCV PROPERTIES IMPORTED_LOCATION ${LIBOPENCV_DIR}/sdk/native/libs/${CMAKE_ANDROID_ARCH_ABI}/libopencv_java4.so)



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
        LIBOPENCV
        ${LIBFACETRACKER}
       )