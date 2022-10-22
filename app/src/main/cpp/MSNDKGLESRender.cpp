
#include "MSNDKGLESRender.h"
#include "MSVAOSample.h"
#include "MSTransitionSample.h"
#include "MSHighLightSample.h"
#include "MSBurnPageSample.h"

/**
 * VBO EBO VAO 最佳实践
 */
MSNDKGLESRender::MSNDKGLESRender() : m_pAssetManager(nullptr) {
//    m_msBaseSample = new MSVAOSample();  //绘制立方体
//    m_msBaseSample = new MSTransitionSample();   //绘制shader动画
//    m_msBaseSample = new MSHighLightSample();   //绘制高光效果
    m_msBaseSample = new MSBurnPageSample();   //绘制燃烧效果
}

MSNDKGLESRender::~MSNDKGLESRender() {
    if (m_msBaseSample){
        delete m_msBaseSample;
        m_msBaseSample = nullptr;
    }
}

void MSNDKGLESRender::InitGL() {
  m_msBaseSample->InitGL();
    if (m_pAssetManager != NULL) {
        loadTextureResources(m_pAssetManager);
        loadShaderResources(m_pAssetManager);
    }
    setupRenderScreenSize();
    setupRenderingObject();
}

void MSNDKGLESRender::PaintGL() {
    m_msBaseSample->PaintGL();
}

void MSNDKGLESRender::ResizeGL(int width, int height) {
    m_msBaseSample->ResizeGL(width,height);
}

void MSNDKGLESRender::SetupAssetManager(AAssetManager *pManager) {
    if (pManager == NULL) {
        return;
    }
    m_pAssetManager = pManager;

}

void MSNDKGLESRender::loadTextureResources(AAssetManager *pManager) {
   m_msBaseSample->loadTextureResources(pManager);
}

void MSNDKGLESRender::loadShaderResources(AAssetManager *pManager) {
   m_msBaseSample->loadShaderResources(pManager);
}

void MSNDKGLESRender::setupFrameBufferObject() {
    m_msBaseSample->setupFrameBufferObject();
}

/**
 *  在渲染前设置缓冲数据
 */
void MSNDKGLESRender::setupRenderingObject() {
   m_msBaseSample->setupRenderingObject();
}

void MSNDKGLESRender::setupRenderScreenSize() {
   m_msBaseSample->setupRenderScreenSize();
}
