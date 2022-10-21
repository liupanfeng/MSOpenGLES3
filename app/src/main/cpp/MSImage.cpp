
#include "MSImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdlib.h>
#include <string.h>


MSImage::MSImage(): m_width(0), m_height(0), m_type(0), m_pImgData(NULL)
{

}

MSImage::~MSImage()
{
    if(m_pImgData)
    {
        free(m_pImgData);
    }
}


void MSImage::ReadFromFile(U8_t* fileName)
{
    int			type = 0;
    int			width = 0;
    int			height = 0;

    //stbi_set_flip_vertically_on_load(true);

    U8_t* picData = stbi_load((char const *)fileName, &width, &height, &type, STBI_rgb_alpha);

    int imgSize = width * height * 4;

    if(imgSize >0 && picData != nullptr){

        m_pImgData =(U8_t*) malloc(imgSize);
        memcpy(m_pImgData,picData,imgSize);

        m_width =width;
        m_height =height;
        m_type = type;
    }

    stbi_image_free(picData);

}

void MSImage::ReadFromBuffer(U8_t* dataBuff, int length)
{
    int			type = 0;
    int			width = 0;
    int			height = 0;

    stbi_set_flip_vertically_on_load(true);

    U8_t* picData = stbi_load_from_memory((U8_t const *)dataBuff, length, &width, &height, &type, 0);

    int imgSize = width * height * 4;

    if(imgSize >0 && picData != nullptr){

        m_pImgData =(U8_t*) malloc(imgSize);
        memcpy(m_pImgData,picData,imgSize);

        m_width =width;
        m_height =height;
        m_type = type;
    }

    stbi_image_free(picData);

}


int MSImage::GetWidth() const
{
    return m_width;
}

int MSImage::GetHeight() const
{
    return m_height;
}

int MSImage::GetType() const
{
    return m_type;
}

U8_t* MSImage::GetData() const
{
    return m_pImgData;
}