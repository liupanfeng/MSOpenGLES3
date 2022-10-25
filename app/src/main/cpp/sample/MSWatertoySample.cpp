
#include "MSWatertoySample.h"

MSWatertoySample::MSWatertoySample() {
    Init();
}

MSWatertoySample::~MSWatertoySample() {
    glDeleteTextures(6,m_texID);
    MSGLSafePtrDelete(m_pVBO);
    MSGLSafePtrDelete(m_pEBO);
}

void MSWatertoySample::Init() {
    MSBaseSample::Init();
}

void MSWatertoySample::InitGL() {
    MSBaseSample::InitGL();
}

void MSWatertoySample::PaintGL() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_nValue += 0.05f;
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
    glBindTexture(GL_TEXTURE_2D,m_texID[1]);

    m_pOpenGLShader->SetUniformValue("iChannel1",1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,m_texID[2]);


    m_pVAO->Bind();

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,NULL);

    m_pOpenGLShader->Release();
    m_pVAO->Release();

    glBindTexture(GL_TEXTURE_2D,0);
}

void MSWatertoySample::ResizeGL(int w, int h) {
    glViewport(0,0,w,h);
}

void MSWatertoySample::loadTextureResources(AAssetManager *pManager) {
    MSBaseSample::loadTextureResources(pManager);
}

void MSWatertoySample::loadShaderResources(AAssetManager *pManager) {
    m_pOpenGLShader->InitShadersFromFile(pManager,"waterv.glsl","waterf.glsl");
}

void MSWatertoySample::setupRenderingObject() {
    if(m_pOpenGLShader == NULL){
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
    m_pVBO->SetBufferData(planeVertexs,sizeof(planeVertexs));

    m_pEBO->Create();
    m_pEBO->Bind();
    m_pEBO->SetBufferData(planeIndexs,sizeof(planeIndexs));

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

void MSWatertoySample::setupRenderScreenSize() {
    MSBaseSample::setupRenderScreenSize();
}
