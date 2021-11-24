#pragma once
#include <Shaders.hpp>
#include <Texture.hpp>
#include <vector>
#include <glm/glm.hpp>


class Cube
{
public:
    Cube();
    void initCubeBuffers();
    void drawCube(unsigned int p_textureNumber, glm::mat4& p_mvp);
    void setAttributes(SShaderAttributes &p_attributes);

private:
    const static GLushort m_cubeElements[48];
    const static GLfloat m_cubeTexcords[48];
    const static GLfloat m_cubeVertices[72];

    Shaders m_shaders;
    GLuint m_vboCubeTexcoords;
    GLuint m_vboCubeVertices;
    GLuint m_iboCubeElements;
    std::vector<Texture*> m_textures;
};
