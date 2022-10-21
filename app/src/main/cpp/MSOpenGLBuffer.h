
#ifndef MSOPENGLES_MSOPENGLBUFFER_H
#define MSOPENGLES_MSOPENGLBUFFER_H

#include "MSGLCommonDef.h"
/**
 * VBO 、EBO VAO 等高级缓冲区的封装
 */
class MSOpenGLBuffer {

public:

    enum Type{
        VertexBuffer        = 0x8892, // GL_ARRAY_BUFFER
        IndexBuffer         = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
        PixelPackBuffer     = 0x88EB, // GL_PIXEL_PACK_BUFFER
        PixelUnpackBuffer   = 0x88EC  // GL_PIXEL_UNPACK_BUFFER
    };

    enum UsagePattern
    {
        StreamDraw          = 0x88E0, // GL_STREAM_DRAW
        StreamRead          = 0x88E1, // GL_STREAM_READ
        StreamCopy          = 0x88E2, // GL_STREAM_COPY
        StaticDraw          = 0x88E4, // GL_STATIC_DRAW
        StaticRead          = 0x88E5, // GL_STATIC_READ
        StaticCopy          = 0x88E6, // GL_STATIC_COPY
        DynamicDraw         = 0x88E8, // GL_DYNAMIC_DRAW
        DynamicRead         = 0x88E9, // GL_DYNAMIC_READ
        DynamicCopy         = 0x88EA  // GL_DYNAMIC_COPY
    };


    MSOpenGLBuffer(MSOpenGLBuffer::Type type, MSOpenGLBuffer::UsagePattern usage);
    ~MSOpenGLBuffer();

    void Bind();
    void Release();
    void Create();
    void SetBufferData(const GLvoid *data, GLsizeiptr size);

private:
    MSOpenGLBuffer::Type   m_bufferType;
    GLuint              m_buffID;
    GLsizeiptr          m_buffSize;
    MSOpenGLBuffer::UsagePattern              m_usage;
};


#endif //MSOPENGLES_MSOPENGLBUFFER_H
