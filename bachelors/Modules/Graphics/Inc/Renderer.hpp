#ifndef RENDERER_H
#define RENDERER_H

#include <ModelView.hpp>
#include <Controlers.hpp>
#include <ModelController.hpp>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>


class Renderer
{
public:
    Renderer(Controlers* p_controlers, SDL_Window* p_window);
    void onDisplay();

private:
    void calculateCameraViewMatrix();

    glm::mat4 m_cameraViewMatrixMatrix = glm::mat4(1.0);
    Controlers* m_controlers;
    SDL_Window* m_window;
};

#endif // RENDERER_H
