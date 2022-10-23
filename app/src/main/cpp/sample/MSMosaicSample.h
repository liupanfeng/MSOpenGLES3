//
// Created by 56930 on 2022/10/23.
//

#ifndef MSOPENGLES3_MSMOSAICSAMPLE_H
#define MSOPENGLES3_MSMOSAICSAMPLE_H

#include "MSBaseSample.h"

class MSMosaicSample : public MSBaseSample{
public:
    MSMosaicSample();
    virtual ~MSMosaicSample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();
    virtual void setupRenderScreenSize();
};


#endif //MSOPENGLES3_MSMOSAICSAMPLE_H
