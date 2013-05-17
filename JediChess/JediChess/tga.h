//*********************************************
// tga.h
//*********************************************

#ifndef JediChess_tgaimage_h
#define JediChess_tgaimage_h

class TgaImage
{
public:
    TgaImage()
    {
        data = (unsigned char*)0;
        width = 0;
        height = 0;
        byteCount = 0;
    }

    ~TgaImage() { delete[] data; data = 0; }

    bool loadTGA(const char* filename);

    int width;
    int height;
    unsigned char byteCount;
    unsigned char* data;
};

#endif