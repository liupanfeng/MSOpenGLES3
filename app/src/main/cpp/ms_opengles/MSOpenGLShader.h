
#ifndef MSOPENGLES_MSOPENGLSHADER_H
#define MSOPENGLES_MSOPENGLSHADER_H

#include "MSGLCommonDef.h"

class MSOpenGLShader {

public:
    MSOpenGLShader();
    ~MSOpenGLShader();

    void Bind();
    void Release();

    void InitShadersFromFile(AAssetManager*  pManager, const char* vShader,const char* fshader);

    void DisableAttributeArray(const char *name);
    void EnableAttributeArray(const char *name);
    void SetAttributeBuffer(const char* name,GLenum type, const void *values, int tupleSize, int stride = 0);

    void SetUniformValue(const char* name, int iValue);
    void SetUniformValue(const char* name, GLfloat fValue);
    void SetUniformValue(const char* name, glm::vec2 vecValue);
    void SetUniformValue(const char* name, glm::vec3 vecValue);
    void SetUniformValue(const char* name, glm::mat4 matValue);


    void DisableAttributeArray(int location);
    void EnableAttributeArray(int location);
    void SetAttributeBuffer(int location,GLenum type, const void *values, int tupleSize, int stride = 0);

private:
    int compileShader(AAssetManager*  m_pAssetManager,const char* sPath, GLint sType);
private:
    GLuint m_shaderProgram;
};


#endif //MSOPENGLES_MSOPENGLSHADER_H
