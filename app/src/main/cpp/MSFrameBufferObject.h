
#ifndef MSOPENGLES3_MSFRAMEBUFFEROBJECT_H
#define MSOPENGLES3_MSFRAMEBUFFEROBJECT_H

#include "MSGLCommonDef.h"
/**
 * 帧缓冲区封装
 */
class MSFrameBufferObject {

public:
    MSFrameBufferObject();
    ~MSFrameBufferObject();

    void Bind();
    void Release();
    GLuint GetTextureID();

    void GenerateFBOWithSize(int width, int height);

private:
    GLuint      m_fboId;
    GLuint      m_renderBufferId;
    GLuint      m_textureId;
};


#endif //MSOPENGLES3_MSFRAMEBUFFEROBJECT_H
