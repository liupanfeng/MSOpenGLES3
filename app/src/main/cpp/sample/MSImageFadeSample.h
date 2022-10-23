//
// Created by 56930 on 2022/10/23.
//

#ifndef MSOPENGLES3_MSIMAGEFADESAMPLE_H
#define MSOPENGLES3_MSIMAGEFADESAMPLE_H


#include "MSBaseSample.h"

class MSImageFadeSample : public MSBaseSample{
public:
    MSImageFadeSample();
    virtual ~MSImageFadeSample();

    virtual void Init();
    virtual void InitGL();
    virtual void PaintGL();
    virtual void ResizeGL(int w, int h);

    virtual void loadTextureResources(AAssetManager *pManager);
    virtual void loadShaderResources(AAssetManager *pManager);
    virtual void setupRenderingObject();
};


#endif //MSOPENGLES3_MSIMAGEFADESAMPLE_H
