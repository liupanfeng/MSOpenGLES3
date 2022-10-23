

#ifndef MSOPENGLES3_MSTRANSITIONSAMPLE_H
#define MSOPENGLES3_MSTRANSITIONSAMPLE_H


#include "MSBaseSample.h"

class MSTransitionSample : public MSBaseSample {
public:
    MSTransitionSample();
    virtual ~MSTransitionSample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();

};


#endif //MSOPENGLES3_MSTRANSITIONSAMPLE_H
