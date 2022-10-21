//
// Created by ms on 2022/10/21.
//

#ifndef MSOPENGLES3_MSNDKGLESRENDER_H
#define MSOPENGLES3_MSNDKGLESRENDER_H

#include "MSGLCommonDef.h"
#include "MSOpenGLBuffer.h"
#include "MSFrameBufferObject.h"
#include "MSOpenGLShader.h"
#include "MSOpenGLVAO.h"

class MSNDKGLESRender {
public:
    MSNDKGLESRender();
    ~MSNDKGLESRender();

    void InitGL();
    void PaintGL();
    void ResizeGL(int w, int h);

    void SetupAssetManager(AAssetManager *pManager);

private:
    void loadTextureResources(AAssetManager *pManager);
    void loadShaderResources(AAssetManager *pManager);
    void setupFrameBufferObject();
    void setupRenderingObject();
    void renderTriangle();
private:
    float m_angle =0.0f;
    GLuint m_texID[6];

    AAssetManager*        m_pAssetManager;
    /*顶点缓冲对象*/
    MSOpenGLBuffer*       m_pVBO;
    /*序列缓冲对象*/
    MSOpenGLBuffer*       m_pEBO;
    /*离屏渲染*/
    MSFrameBufferObject*  m_pFBO;
    /*VAO缓冲区*/
    MSOpenGLVAO*          m_pVAO;
    /*shader 操作帮助类*/
    MSOpenGLShader*       m_pOpenGLShader;
};


#endif //MSOPENGLES3_MSNDKGLESRENDER_H
