#include <Graphics.hpp>
#include <ModelController.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>


void Graphics::mainLoop()
{
    while (true)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                m_controlers->onKeyboard(ev.key.keysym.scancode);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_controlers->onMouse(true, ev.motion.x, ev.motion.y);
                break;
            case SDL_MOUSEBUTTONUP:
                m_controlers->onMouse(false, ev.motion.x, ev.motion.y);
                break;
            case SDL_MOUSEMOTION:
                m_controlers->mouseFollower(ev.motion.x, ev.motion.y);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                m_controlers->onResize(ev.window.data1, ev.window.data2);
                break;
            }
        }
        m_renderer->onDisplay();
    }
}

void Graphics::createWindow()
{
    m_window = SDL_CreateWindow("Geometry",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_controlers->m_screenWidth, m_controlers->m_screenHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if(!m_window)
        std::cout << "Failed to create SDL window.\n";
    SDL_GL_CreateContext(m_window);
}

void Graphics::initGraphics(VMoves p_moves, VJointsList p_jointsList, VElements p_modelElements)
{
    m_controlers = new Controlers();
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return;
    }
    createWindow();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    if(glewInit() != GLEW_OK)
        return;
    ModelController l_model(p_moves, p_jointsList, p_modelElements);
    m_controlers->setModel(&l_model);
    m_renderer = new Renderer(m_controlers, m_window);
    mainLoop();
}
