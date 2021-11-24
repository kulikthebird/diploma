#pragma once
#include <GL/glew.h>
#include <GL/gl.h>


struct SShaderAttributes
{
    GLint m_attrCoord3D;
    GLint m_attrTexcoord;
    GLint m_uniformTexture;
    GLint m_uniformMvp;
    GLuint m_graphicProgram;
};

class Shaders
{
public:
    void initializeShaders();
    GLuint initVertexShader();
    GLuint initFragmentShader();
    const SShaderAttributes& getAttributes();

private:
    bool bindAttributesAndUniforms();
    void setupProgram();

    const static GLchar* m_fragmentShader;
    const static GLchar* m_vertexShader;
    GLuint m_vs;
    GLuint m_fs;
    SShaderAttributes m_attr;
};
