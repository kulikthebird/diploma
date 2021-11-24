#pragma once
#include <GL/gl.h>


class Texture
{
public:
    Texture() {}
    Texture(const char* p_pathToBitmapFile);
    unsigned char* readBmp(const char* p_pathToBitmapFile);
    void createTexture(unsigned char* p_bmp);
    void loadTexture(const char* p_pathToBitmapFile);
    GLuint getTexture();

private:
    unsigned int m_width;
    unsigned int m_height;
    GLuint m_textureId;
};
