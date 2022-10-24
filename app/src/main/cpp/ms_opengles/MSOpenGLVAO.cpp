//
// Created by ms on 2022/10/21.
//

#include "MSOpenGLVAO.h"

MSOpenGLVAO::MSOpenGLVAO() {

}

MSOpenGLVAO::~MSOpenGLVAO() {
    glDeleteVertexArrays(1,&m_vaoId);
}

void MSOpenGLVAO::Bind() {
    glBindVertexArray(m_vaoId);
}

void MSOpenGLVAO::Release() {
    glBindVertexArray(0);
}

void MSOpenGLVAO::Create() {
    glGenVertexArrays(1,&m_vaoId);
}
