//
// Created by 56930 on 2022/10/22.
//

#ifndef MSOPENGLES3_MSBASESAMPLE_H
#define MSOPENGLES3_MSBASESAMPLE_H

#include "MSGLCommonDef.h"
#include "MSOpenGLShader.h"
#include "MSOpenGLVAO.h"
#include "MSOpenGLBuffer.h"
#include "MSGLTexture.h"
#include "MSFrameBufferObject.h"

class MSBaseSample {

public:
     MSBaseSample(){}

    virtual ~MSBaseSample(){}

    virtual void InitGL()=0;

    virtual void PaintGL()=0;

    virtual void ResizeGL(int w, int h)=0;

    virtual void loadShaderResources(AAssetManager *pManager)=0;


    virtual void setupRenderingObject()=0;

    virtual void loadTextureResources(AAssetManager *pManager){
        for (int i = 0; i < 6; i++) {
            char nameBuff[6];
            memset(nameBuff, 0, sizeof(nameBuff));
            sprintf(nameBuff, "%d.png", i + 1);
            nameBuff[5] = '\0';
            LOGD("Image Name:%s", nameBuff);
            MSGLTexture glTexture;
            m_texID[i] = glTexture.CreateGLTextureFromFile(pManager, nameBuff);
        }
    }

    virtual void setupFrameBufferObject(){}


protected:
    /*VAO缓冲区*/
    MSOpenGLVAO *m_pVAO;
    /*顶点缓冲对象*/
    MSOpenGLBuffer *m_pVBO;
    /*序列缓冲对象*/
    MSOpenGLBuffer *m_pEBO;
    /*shader 操作帮助类*/
    MSOpenGLShader *m_pOpenGLShader;

    float m_angle = 0.0f;
    float m_nValue = 0.0f;
    GLuint m_texID[6];

    /*离屏渲染*/
    MSFrameBufferObject *m_pFBO;

};


#endif //MSOPENGLES3_MSBASESAMPLE_H
