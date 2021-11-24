#include <Texture.hpp>
#include <iostream>
#include <fstream>


Texture::Texture(const char* p_pathToBitmapFile)
{
    loadTexture(p_pathToBitmapFile);
}

unsigned char* Texture::readBmp(const char* p_pathToBitmapFile)
{
    FILE* f = fopen(p_pathToBitmapFile, "rb");
    if(f == nullptr)
    {
        std::cerr << "Texture file '" << p_pathToBitmapFile << "' does not exist!" << std::endl;
        return nullptr;
    }
    unsigned char l_bmpHeader[54];
    std::fread(l_bmpHeader, sizeof(unsigned char), 54, f);
    m_width = *(int*)&l_bmpHeader[18];
    m_height = *(int*)&l_bmpHeader[22];
    uint l_size = 3 * m_width * m_height;
    unsigned char* l_data = new unsigned char[l_size];
    if(fread(l_data, sizeof(unsigned char), l_size, f) != l_size)
        std::cerr << "Could not load texture file: " << p_pathToBitmapFile << std::endl;
    std::fclose(f);
    return l_data;
}

void Texture::createTexture(unsigned char* p_bmpData)
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, p_bmpData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::loadTexture(const char* p_pathToImageFile)
{
    unsigned char* l_bmpData = readBmp(p_pathToImageFile);
    createTexture(l_bmpData);
    delete l_bmpData;
}

GLuint Texture::getTexture()
{
    return m_textureId;
}
