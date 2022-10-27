
#include "MSNDKGLESRender.h"
#include "../sample/MSVAOSample.h"
#include "../sample/MSTransitionSample.h"
#include "../sample/MSHighLightSample.h"
#include "../sample/MSBurnPageSample.h"
#include "../sample/MSImageFadeSample.h"
#include "../sample/MSMosaicSample.h"
#include "../sample/MSWatertoySample.h"
#include "../sample/MSMosaicSample.h"

#define MS_VAO_SAMPLE               100
#define MS_TRANSITION_SAMPLE        101
#define MS_HIGH_LIGHT_SAMPLE        102
#define MS_BURN_PAGE_SAMPLE         103
#define MS_IMAGE_FADE_SAMPLE        104
#define MS_MOSAIC_SAMPLE            105
#define MS_WATER_SAMPLE             106

/**
 * VBO EBO VAO 最佳实践
 */
MSNDKGLESRender::MSNDKGLESRender(int type) : m_pAssetManager(nullptr) {
    switch (type) {
        case MS_VAO_SAMPLE:
            m_msBaseSample = new MSVAOSample();  //绘制立方体
            break;
        case MS_TRANSITION_SAMPLE:
            m_msBaseSample = new MSTransitionSample();   //绘制shader动画
            break;
        case MS_HIGH_LIGHT_SAMPLE:
            m_msBaseSample = new MSHighLightSample();   //绘制高光效果
            break;
        case MS_BURN_PAGE_SAMPLE:
            m_msBaseSample = new MSBurnPageSample();   //绘制燃烧效果
            break;
        case MS_IMAGE_FADE_SAMPLE:
            m_msBaseSample = new MSImageFadeSample();   //绘制渐隐效果
            break;
        case MS_MOSAIC_SAMPLE:
            m_msBaseSample = new MSMosaicSample();   //绘制模糊效果
            break;
        case MS_WATER_SAMPLE:
            m_msBaseSample = new MSWatertoySample();   //绘制水波纹效果
            break;
        default:
            m_msBaseSample = new MSWatertoySample();   //绘制水波纹效果
            break;
    }

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
