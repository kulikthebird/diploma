#include <Renderer.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer(Controlers* p_controlers, SDL_Window* p_window) : m_controlers(p_controlers), m_window(p_window)
{}

void Renderer::onDisplay()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uint l_currentTime = SDL_GetTicks();
    calculateCameraViewMatrix();
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, m_controlers->m_distance, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*m_controlers->m_screenWidth/m_controlers->m_screenHeight, 0.1f, 1000.0f);
    glm::mat4 mvp = projection * view * m_cameraViewMatrixMatrix;
    m_controlers->m_modelController->renderModel(mvp, l_currentTime);
    SDL_GL_SwapWindow(m_window);
}

void Renderer::calculateCameraViewMatrix()
{
    m_cameraViewMatrixMatrix =
        glm::rotate(glm::mat4(1.0f), m_controlers->m_XAngle, glm::vec3(0, 0, 1)) *
        glm::rotate(glm::mat4(1.0f), -m_controlers->m_YAngle, glm::vec3(1, 0, 0)) * m_cameraViewMatrixMatrix;
    m_controlers->m_XAngle *= 0.75;
    m_controlers->m_YAngle *= 0.75;
}
