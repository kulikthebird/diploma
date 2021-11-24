#include <Cube.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>


Cube::Cube()
{
    m_shaders.initializeShaders();
    m_textures.push_back(new Texture("bright_wood.bmp"));
    m_textures.push_back(new Texture("dark_wood.bmp"));
    initCubeBuffers();
}

void Cube::initCubeBuffers()
{
    glGenBuffers(1, &m_vboCubeTexcoords);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeTexcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeTexcords), m_cubeTexcords, GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboCubeElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboCubeElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_cubeElements), m_cubeElements, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vboCubeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices), m_cubeVertices, GL_STATIC_DRAW);
}

void Cube::drawCube(unsigned int p_textureNumber, glm::mat4& p_mvp)
{
    const SShaderAttributes& l_attr = m_shaders.getAttributes();
    glUseProgram(l_attr.m_graphicProgram);
    glUniformMatrix4fv(l_attr.m_uniformMvp, 1, GL_FALSE, glm::value_ptr(p_mvp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[p_textureNumber]->getTexture());
    glUniform1i(l_attr.m_uniformTexture, /*GL_TEXTURE*/0);

    glEnableVertexAttribArray(l_attr.m_attrCoord3D);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeVertices);
    glVertexAttribPointer(l_attr.m_attrCoord3D,3,GL_FLOAT,GL_FALSE,0,0);

    glEnableVertexAttribArray(l_attr.m_attrTexcoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeTexcoords);
    glVertexAttribPointer(l_attr.m_attrTexcoord,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboCubeElements);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(l_attr.m_attrCoord3D);
    glDisableVertexAttribArray(l_attr.m_attrTexcoord);
}

const GLushort Cube::m_cubeElements[] =
{
    // front
     0,  1,  2,
     2,  3,  0,
    // top
     4,  5,  6,
     6,  7,  4,
    // back
     8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20
};

const GLfloat Cube::m_cubeTexcords[] =
{
    // front
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    // top
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    // back
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    // bottom
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    // left
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    // right
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0
};

const GLfloat Cube::m_cubeVertices[] =
{
    // front
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // top
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    // back
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    // bottom
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // left
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    // right
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f
};
