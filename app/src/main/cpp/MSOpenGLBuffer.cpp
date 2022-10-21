
#include "MSOpenGLBuffer.h"

MSOpenGLBuffer::MSOpenGLBuffer(MSOpenGLBuffer::Type type, MSOpenGLBuffer::UsagePattern usage)
                        :m_bufferType(type),m_usage(usage),m_buffSize(0)
{
    glGenBuffers(1, &m_buffID);

}

MSOpenGLBuffer::~MSOpenGLBuffer()
{
    glDeleteBuffers(1, &m_buffID);
}

void MSOpenGLBuffer::Bind()
{
    glBindBuffer(m_bufferType, m_buffID);

}

void MSOpenGLBuffer::Release()
{
    glBindBuffer(m_bufferType, 0);
}

void MSOpenGLBuffer::SetBufferData(const GLvoid *data, GLsizeiptr size)
{
    if (size > m_buffSize) {
        m_buffSize = size;
        glBufferData(m_bufferType, size, data, m_usage);
    } else {
        glBufferSubData(m_bufferType, 0, size, data);
    }
}

void MSOpenGLBuffer::Create() {
    glGenBuffers(1,&m_buffID);
}
