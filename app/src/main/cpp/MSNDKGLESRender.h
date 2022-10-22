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

#include "MSBaseSample.h"

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
private:
    MSBaseSample* m_msBaseSample;
    AAssetManager*        m_pAssetManager;
};


#endif //MSOPENGLES3_MSNDKGLESRENDER_H
