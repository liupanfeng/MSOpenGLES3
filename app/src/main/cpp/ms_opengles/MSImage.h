
#ifndef MSOPENGLES_MSIMAGE_H
#define MSOPENGLES_MSIMAGE_H


#ifndef U8_t
#define U8_t unsigned char
#endif


class MSImage {

public:
    MSImage();
    ~MSImage();

    int         GetWidth() const;
    int         GetHeight() const;
    int         GetType() const ;
    U8_t*       GetData() const;

    void        ReadFromFile(U8_t* fileName);
    void        ReadFromBuffer(U8_t* dataBuff,int length);

private:
    int         m_width;
    int         m_height;
    int         m_type;
    U8_t*       m_pImgData;
};


#endif //MSOPENGLES_CCIMAGE_H
