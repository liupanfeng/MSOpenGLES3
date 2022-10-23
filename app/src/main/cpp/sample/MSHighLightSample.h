//
// Created by 56930 on 2022/10/22.
//

#ifndef MSOPENGLES3_MSHIGHLIGHTSAMPLE_H
#define MSOPENGLES3_MSHIGHLIGHTSAMPLE_H

#include "MSBaseSample.h"

class MSHighLightSample :public MSBaseSample {
public:
    MSHighLightSample();
    virtual ~MSHighLightSample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();

    virtual void setupRenderScreenSize();

};


#endif //MSOPENGLES3_MSHIGHLIGHTSAMPLE_H
