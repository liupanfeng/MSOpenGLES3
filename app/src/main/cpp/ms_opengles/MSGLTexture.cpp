
#include "MSGLTexture.h"

MSGLTexture::MSGLTexture()
{
}

MSGLTexture::~MSGLTexture()
{
}

GLuint MSGLTexture::GetTextureID()
{
    return m_texID;
}

GLuint  MSGLTexture::CreateGLTextureFromFile(AAssetManager *assetManager, const char* fileName)
{
    m_texID = generateTexture(assetManager,fileName);
    return m_texID;
}

GLuint  MSGLTexture::generateTexture(AAssetManager *assetManager, const char* fileName)
{
    AAsset *asset = AAssetManager_open (assetManager, fileName, AASSET_MODE_UNKNOWN);
    if (NULL == asset){
        LOGE("asset is NULL");
        return -1;
    }
    off_t bufferSize = AAsset_getLength(asset);
    LOGD("buffer size is %ld", bufferSize);

    unsigned char *imgBuff = (unsigned char *)malloc(bufferSize + 1);
    if (NULL == imgBuff){
        LOGE("imgBuff alloc failed");
        return -1;
    }
    memset(imgBuff, 0, bufferSize + 1);
    int readLen = AAsset_read(asset, imgBuff, bufferSize);
    LOGD("Picture read: %d", readLen);

    MSImage* glImage = new MSImage();
    glImage->ReadFromBuffer(imgBuff,readLen);
    GLuint texID = createOpenGLTexture(glImage);

    delete glImage;

    if (imgBuff){
        free(imgBuff);
        imgBuff = NULL;
    }

    AAsset_close(asset);

    return texID;
}

GLuint MSGLTexture::createOpenGLTexture(MSImage* pImg)
{
    if(pImg == NULL){
        return -1;
    }

    GLuint textureID;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1,&textureID);//产生纹理索引
    glBindTexture(GL_TEXTURE_2D,textureID);//绑定纹理索引，之后的操作都针对当前纹理索引

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//指当纹理图象被使用到一个大于它的形状上时
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//指当纹理图象被使用到一个小于或等于它的形状上时
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,pImg->GetWidth(),pImg->GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,pImg->GetData());//指定参数，生成纹理



//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //先是一个字节对齐   GL_LUMINANCE：表示亮度
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nVideoW, m_nVideoH, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pBufYuv420p);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //然后成4个字节对齐
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}