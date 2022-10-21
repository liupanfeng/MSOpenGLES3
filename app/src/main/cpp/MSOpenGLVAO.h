//
// Created by ms on 2022/10/21.
//

#ifndef MSOPENGLES3_MSOPENGLVAO_H
#define MSOPENGLES3_MSOPENGLVAO_H

#include "MSGLCommonDef.h"

class MSOpenGLVAO {
public:
    MSOpenGLVAO();
    ~MSOpenGLVAO();

    void Bind();
    void Release();
    void Create();

private:
    GLuint  m_vaoId;
};


#endif //MSOPENGLES3_MSOPENGLVAO_H
