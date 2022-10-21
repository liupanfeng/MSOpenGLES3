//
// Created by ms on 2022/10/21.
//

#include "MSFrameBufferObject.h"

MSFrameBufferObject::MSFrameBufferObject() {

}

MSFrameBufferObject::~MSFrameBufferObject() {
    glDeleteTextures(1,&m_textureId);
}

void MSFrameBufferObject::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER,m_fboId);
}

void MSFrameBufferObject::Release() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

GLuint MSFrameBufferObject::GetTextureID() {
    return m_textureId;
}

/**
 * 创建指定宽高的FBO 对象
 * @param width
 * @param height
 */
void MSFrameBufferObject::GenerateFBOWithSize(int width, int height) {
    glGenTextures(1,&m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width,height,0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D,0);

    glGenRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,height);
    glBindRenderbuffer(GL_RENDERBUFFER,0);


    glGenFramebuffers(1, &m_fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureId,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId);

    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        LOGD("Framebuffer creation successful\n");
    } else {
        LOGE("Error creating framebuffer [status: %d]\n", status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}


