#pragma once

#include <BasicStructures.hpp>
#include <Renderer.hpp>
#include <Controlers.hpp>
#include <SDL2/SDL.h>


class Graphics
{
public:
    void initGraphics(VMoves p_moves, VJointsList p_jointsList, VElements p_modelElements);

private:
    void mainLoop();
    void createWindow();

    Controlers* m_controlers;
    Renderer* m_renderer;
    SDL_Window* m_window;
};
