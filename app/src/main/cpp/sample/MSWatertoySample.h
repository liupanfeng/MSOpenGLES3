
#ifndef MSOPENGLES3_MSWATERTOYSAMPLE_H
#define MSOPENGLES3_MSWATERTOYSAMPLE_H

#include "MSBaseSample.h"

class MSWatertoySample : public MSBaseSample{
public:
    MSWatertoySample();
    virtual ~MSWatertoySample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();

    virtual void setupRenderScreenSize();
};


#endif //MSOPENGLES3_MSWATERTOYSAMPLE_H
