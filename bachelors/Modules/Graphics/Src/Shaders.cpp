#include <Shaders.hpp>
#include <iostream>


void Shaders::initializeShaders()
{
    m_vs = initVertexShader();
    m_fs = initFragmentShader();
    setupProgram();
    bindAttributesAndUniforms();
}

bool Shaders::bindAttributesAndUniforms()
{
    glUseProgram(m_attr.m_graphicProgram);
    const char* name;
    name = "coord3d";
    m_attr.m_attrCoord3D = glGetAttribLocation(m_attr.m_graphicProgram, name);
    if (m_attr.m_attrCoord3D == -1)
    {
        std::cout << "Could not bind attribute " << name << std::endl;
        return false;
    }
    name = "texcoord";
    m_attr.m_attrTexcoord = glGetAttribLocation(m_attr.m_graphicProgram, name);
    if (m_attr.m_attrTexcoord == -1)
    {
        std::cout << "Could not bind attribute " << name << std::endl;
        return false;
    }
    name = "mvp";
    m_attr.m_uniformMvp = glGetUniformLocation(m_attr.m_graphicProgram, name);
    if (m_attr.m_uniformMvp == -1)
    {
        std::cout << "Could not bind attribute " << name << std::endl;
        return false;
    }
    name = "mytexture";
    m_attr.m_uniformTexture = glGetUniformLocation(m_attr.m_graphicProgram, name);
    if (m_attr.m_uniformTexture == -1) {
        std::cout << "Could not bind attribute " << name << std::endl;
        return false;
    }
    return true;
}

void Shaders::setupProgram()
{
    m_attr.m_graphicProgram = glCreateProgram();
    GLint linkOk = GL_FALSE;
    glAttachShader(m_attr.m_graphicProgram, m_vs);
    glAttachShader(m_attr.m_graphicProgram, m_fs);
    glLinkProgram(m_attr.m_graphicProgram);
    glGetProgramiv(m_attr.m_graphicProgram, GL_LINK_STATUS, &linkOk);
    if (!linkOk)
        std::cerr << "Error in glLinkProgram" << std::endl;
}

GLuint Shaders::initVertexShader()
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLint compileOk = GL_FALSE;
    glShaderSource(vs, 1, &m_vertexShader, 0);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compileOk);
    if (!compileOk) {
        std::cerr << "Error in vertex shader" << std::endl;
        return 0;
    }
    return vs;
}

GLuint Shaders::initFragmentShader()
{
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    GLint compileOk = GL_FALSE;
    glShaderSource(fs, 1, &m_fragmentShader, 0);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compileOk);
    if (!compileOk) {
        std::cerr << "Error in fragment shader" << std::endl;
        return 0;
    }
    return fs;
}

const SShaderAttributes& Shaders::getAttributes()
{
    return m_attr;
}

const GLchar* Shaders::m_fragmentShader =
    "varying vec2 f_texcoord;\n\
    uniform sampler2D mytexture;\n\
    void main(void)\n\
    {\n\
        gl_FragColor = texture2D(mytexture, f_texcoord);\n\
    }";

const GLchar* Shaders::m_vertexShader =
    "attribute vec3 coord3d;\n\
    attribute vec2 texcoord;\n\
    varying vec2 f_texcoord;\n\
    uniform mat4 mvp;\n\
    void main(void)\n\
    {\n\
        gl_Position = mvp * vec4(coord3d, 1.0);\n\
        f_texcoord = texcoord;\n\
    }";
