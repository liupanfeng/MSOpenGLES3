

#include "MSHighLightSample.h"

MSHighLightSample::MSHighLightSample() {
    Init();
}

MSHighLightSample::~MSHighLightSample() {

}

void MSHighLightSample::Init() {
    MSBaseSample::Init();
}


void MSHighLightSample::InitGL() {
    MSBaseSample::InitGL();
}



void MSHighLightSample::ResizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MSHighLightSample::loadTextureResources(AAssetManager *pManager) {
    MSBaseSample::loadTextureResources(pManager);
}

void MSHighLightSample::loadShaderResources(AAssetManager *pManager) {
    m_pOpenGLShader->InitShadersFromFile(pManager,"hightlightsv.glsl","hightlightsf.glsl");
}


void MSHighLightSample::setupRenderingObject() {

}

void MSHighLightSample::PaintGL() {

}




