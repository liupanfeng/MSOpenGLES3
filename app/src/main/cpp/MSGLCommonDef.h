
#ifndef MSOPENGLES3_MSGLCOMMONDEF_H
#define MSOPENGLES3_MSGLCOMMONDEF_H
#include <string>

#include <GLES3/gl3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <assert.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "android_log_util.h"

template<typename T>

void MSGLSafePtrDelete(T* ptr)
{
    delete ptr;
    ptr = NULL;
}


class MSGlobalDefine
{
    inline static void PrintGLError()
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            LOGD("glError: %s",err);
        }
    }

};

#ifdef __cplusplus
extern "C" {
#endif


struct  MSFloat7{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
};

struct  MSFloat3{
    float x;
    float y;
    float z;
};

struct  MSFloat4{
    float r;
    float g;
    float b;
    float a;
};

struct  MSFloat5{
    float x;
    float y;
    float z;
    float u;
    float v;
};


#ifdef __cplusplus
}
#endif

#endif //MSOPENGLES3_MSGLCOMMONDEF_H
