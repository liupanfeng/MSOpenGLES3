//
// Created by 56930 on 2022/10/22.
//

#include "MSVAOSample.h"

MSVAOSample::MSVAOSample() {
    Init();
}

MSVAOSample::~MSVAOSample() {
    glDeleteTextures(6, m_texID);
    MSGLSafePtrDelete(m_pVBO);
    MSGLSafePtrDelete(m_pEBO);
}

void MSVAOSample::Init() {
    MSBaseSample::Init();
}

void MSVAOSample::InitGL() {
   MSBaseSample::InitGL();
}

void MSVAOSample::PaintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_angle += 0.05f;

    glm::mat4x4 objectMat;
    glm::mat4x4 cubeTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3));
    glm::mat4x4 cubeRotMat = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(1.0f, 1.0f, 1.0));
    glm::mat4x4 objectScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5));

    glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float) 9 / (float) 18, 0.1f,
                                         1000.0f);
    objectMat = projMat *cubeTransMat*cubeRotMat*objectScaleMat;

    m_pOpenGLShader->Bind();
    m_pOpenGLShader->SetUniformValue("u_mat", objectMat);


//    m_pOpenGLShader->EnableAttributeArray("a_position");
//    m_pOpenGLShader->EnableAttributeArray("a_color");
//    m_pOpenGLShader->SetAttributeBuffer("a_position",GL_FLOAT,triangleVert,3,sizeof(CCFloat7));
//    m_pOpenGLShader->SetAttributeBuffer("a_color",GL_FLOAT,&triangleVert[0].r,4,sizeof(CCFloat7));
//    glDrawArrays(GL_TRIANGLES,0,3);
//
//    m_pOpenGLShader->DisableAttributeArray("a_position");
//    m_pOpenGLShader->DisableAttributeArray("a_color");


    m_pVAO->Bind();

    for (int i = 0; i < 6; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texID[i]);
        int offset = i * 6 * sizeof(unsigned short);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *) offset);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    m_pOpenGLShader->Release();
    m_pVAO->Release();
}

void MSVAOSample::ResizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MSVAOSample::loadTextureResources(AAssetManager *pManager) {
   MSBaseSample::loadTextureResources(pManager);
}

void MSVAOSample::loadShaderResources(AAssetManager *pManager) {
    m_pOpenGLShader->InitShadersFromFile(pManager, "cubev.glsl", "cubef.glsl");
}

void MSVAOSample::setupFrameBufferObject() {
    int viewport[4] = {0};
    glGetIntegerv(GL_VIEWPORT, viewport);
    int sWidth = viewport[2];
    int sHeight = viewport[3];
    if (sWidth > 0 && sHeight > 0) {
        m_pFBO->GenerateFBOWithSize(sWidth, sHeight);
    }
}

void MSVAOSample::setupRenderingObject() {
    if (m_pOpenGLShader == NULL) {
        return;
    }
    const MSFloat5 cubeVertexs[] = {
            {-1.0, -1.0, 1.0,  0.0, 0.0},
            {-1.0, 1.0,  1.0,  0.0, 1.0},
            {1.0,  -1.0, 1.0,  1.0, 0.0},
            {1.0,  1.0,  1.0,  1.0, 1.0},

            {1.0,  -1.0, -1.0, 0,   0},
            {1.0,  1.0,  -1.0, 0,   1},
            {-1.0, -1.0, -1.0, 1,   0},
            {-1.0, 1.0,  -1.0, 1,   1},


            {-1.0, -1.0, -1.0, 0,   0},
            {-1.0, 1.0,  -1.0, 0,   1},
            {-1.0, -1.0, 1.0,  1,   0},
            {-1.0, 1.0,  1.0,  1,   1},

            {1.0,  -1.0, 1.0,  0,   0},
            {1.0,  1.0,  1.0,  0,   1},
            {1.0,  -1.0, -1.0, 1,   0},
            {1.0,  1.0,  -1.0, 1,   1},

            {-1.0, 1.0,  1.0,  0,   0},
            {-1.0, 1.0,  -1.0, 0,   1},
            {1.0,  1.0,  1.0,  1,   0},
            {1.0,  1.0,  -1.0, 1,   1},

            {-1.0, -1.0, -1.0, 0,   0},
            {-1.0, -1.0, 1.0,  0,   1},
            {1.0,  -1.0, -1.0, 1,   0},
            {1.0,  -1.0, 1.0,  1,   1}
    };

    const short cubeIndexs[] = {
            0, 1, 2, 2, 1, 3,
            4, 5, 6, 6, 5, 7,
            8, 9, 10, 10, 9, 11,
            12, 13, 14, 14, 13, 15,
            16, 17, 18, 18, 17, 19,
            20, 21, 22, 22, 21, 23,
    };

    m_pVAO->Create();
    m_pVAO->Bind();

    m_pVBO->Create();
    m_pVBO->Bind();
    m_pVBO->SetBufferData(cubeVertexs, sizeof(cubeVertexs));

    m_pEBO->Create();
    m_pEBO->Bind();
    m_pEBO->SetBufferData(cubeIndexs, sizeof(cubeIndexs));


    int offset = 0;

    /**
     * 如果传位置就会调用 位置的重载函数  如果传名字就会调用名字的重载函数，名称内部会获取location
     */
    m_pOpenGLShader->SetAttributeBuffer(0, GL_FLOAT, (void*)offset, 3,
                                        sizeof(MSFloat5));
    m_pOpenGLShader->EnableAttributeArray(0);


    offset += 3 * sizeof(float);

    m_pOpenGLShader->SetAttributeBuffer(1, GL_FLOAT,(void*)offset, 3,
                                        sizeof(MSFloat5));
    m_pOpenGLShader->EnableAttributeArray(1);

    m_pVAO->Release();
    m_pVBO->Release();
    m_pEBO->Release();
}


