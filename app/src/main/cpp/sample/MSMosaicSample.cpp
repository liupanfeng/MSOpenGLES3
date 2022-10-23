//
// Created by 56930 on 2022/10/23.
//

#include "MSMosaicSample.h"

MSMosaicSample::MSMosaicSample() {
    Init();
    m_nValue=30;
}

MSMosaicSample::~MSMosaicSample() {
    glDeleteTextures(6,m_texID);
    MSGLSafePtrDelete(m_pVBO);
    MSGLSafePtrDelete(m_pEBO);
}

void MSMosaicSample::Init() {
    MSBaseSample::Init();
}

void MSMosaicSample::InitGL() {
    MSBaseSample::InitGL();
}

void MSMosaicSample::PaintGL() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (m_nValue>=160){
        m_nValue=30;
    }
    m_nValue += 0.08f;
    m_angle += 0.01f;

    glm::mat4x4  objectMat;
    glm::mat4x4  objectTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3));
    glm::mat4x4  objectRotMat = glm::rotate(glm::mat4(1.0f),m_angle,glm::vec3(1.0f, 1.0f, 1.0) );
    glm::mat4x4  objectScaleMat = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f, 1.0f, 1.0) );

    glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)9/(float)18, 0.1f, 1000.0f);
    objectMat = projMat* objectTransMat ;


    m_pOpenGLShader->Bind();

    m_pOpenGLShader->SetUniformValue("u_mat",objectMat);
    m_pOpenGLShader->SetUniformValue("iTime",m_nValue);

    m_pOpenGLShader->SetUniformValue("iResolution",glm::vec2(1.0f,1.0f));  //设置二维向量


    m_pOpenGLShader->SetUniformValue("iChannel0",0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_texID[3]);

    m_pOpenGLShader->SetUniformValue("iChannel1",1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,m_texID[3]);


    m_pVAO->Bind();

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,NULL);

    m_pOpenGLShader->Release();
    m_pVAO->Release();

    glBindTexture(GL_TEXTURE_2D,0);
}

void MSMosaicSample::ResizeGL(int w, int h) {
    glViewport(0,0,w,h);
}

void MSMosaicSample::loadTextureResources(AAssetManager *pManager) {
    MSBaseSample::loadTextureResources(pManager);
}

void MSMosaicSample::loadShaderResources(AAssetManager *pManager) {
    m_pOpenGLShader->InitShadersFromFile(pManager,"mosaicv.glsl","mosaicf.glsl");
}

void MSMosaicSample::setupRenderingObject() {
    if (m_pOpenGLShader== nullptr){
        return;
    }

    const MSFloat5 planeVertexs[]  = {
            {  -1.0,-1.0, 0.0 ,  0.0, 0.0 },
            {  -1.0, 1.0, 0.0 ,  0.0, 1.0 },
            {  1.0, -1.0,  0.0 , 1.0, 0.0 },
            {   1.0, 1.0, 0.0 ,  1.0, 1.0 },
    };

    const short planeIndexs[]= {
            0, 1, 2,  1, 3, 2
    };

    m_pVAO->Create();
    m_pVAO->Bind();

    m_pVBO->Create();
    m_pVBO->Bind();
    m_pVBO->SetBufferData(planeVertexs,sizeof planeVertexs);

    m_pEBO->Create();
    m_pEBO->Bind();
    m_pEBO->SetBufferData(planeIndexs,sizeof planeIndexs);

    int offset = 0;

    /*将顶点传递给shader*/
    m_pOpenGLShader->SetAttributeBuffer(0,GL_FLOAT, (void *)offset, 3, sizeof(MSFloat5));
    m_pOpenGLShader->EnableAttributeArray(0);


    offset += 3 * sizeof(float);

    m_pOpenGLShader->SetAttributeBuffer(1,GL_FLOAT, (void *)offset, 2, sizeof(MSFloat5));
    m_pOpenGLShader->EnableAttributeArray(1);


    m_pVAO->Release();
    m_pVBO->Release();
    m_pEBO->Release();
}

void MSMosaicSample::setupRenderScreenSize() {
   MSBaseSample::setupRenderScreenSize();
}
