
#ifndef MSOPENGLES3_MSBURNPAGESAMPLE_H
#define MSOPENGLES3_MSBURNPAGESAMPLE_H

#include "MSBaseSample.h"
class MSBurnPageSample :public MSBaseSample {
public:
    MSBurnPageSample();
    virtual ~MSBurnPageSample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();

//    virtual void setupRenderScreenSize();
};


#endif //MSOPENGLES3_MSBURNPAGESAMPLE_H
