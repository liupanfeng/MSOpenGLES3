
#ifndef MSOPENGLES_MSGLTEXTURE_H
#define MSOPENGLES_MSGLTEXTURE_H

#include "MSGLCommonDef.h"
#include "MSImage.h"

class MSGLTexture {

public:
    MSGLTexture();
    ~MSGLTexture();

    GLuint  GetTextureID();
    GLuint  CreateGLTextureFromFile(AAssetManager *assetManager, const char* fileName);

private:
    GLuint  generateTexture(AAssetManager *assetManager, const char* fileName);
    GLuint  createOpenGLTexture(MSImage* pImg);

private:
    GLuint m_texID;
};


#endif //MSOPENGLES_MSGLTEXTURE_H
