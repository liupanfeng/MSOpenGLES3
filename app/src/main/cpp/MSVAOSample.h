//
// Created by 56930 on 2022/10/22.
//

#ifndef MSOPENGLES3_MSVAOSAMPLE_H
#define MSOPENGLES3_MSVAOSAMPLE_H

#include "MSBaseSample.h"

class MSVAOSample : public MSBaseSample{
public:
    MSVAOSample();
    virtual ~MSVAOSample();

    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupFrameBufferObject();
    virtual void setupRenderingObject();
};


#endif //MSOPENGLES3_MSVAOSAMPLE_H
