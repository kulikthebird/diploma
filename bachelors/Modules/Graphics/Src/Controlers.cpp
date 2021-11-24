#include <Controlers.hpp>


void Controlers::onKeyboard(SDL_Scancode p_key)
{
    switch (p_key)
    {
    case SDL_SCANCODE_S:
        m_distance++;
        break;
    case SDL_SCANCODE_W:
        m_distance--;
        break;
    case SDL_SCANCODE_N:
        m_modelController->nextStep();
        break;
    case SDL_SCANCODE_P:
        m_modelController->stepBack();
       break;
    }
}

void Controlers::onMouse(bool p_isPushed, int p_x, int p_y)
{
    if(p_isPushed)
    {
        m_followTheMouse = true;
        m_oldXpos = p_x;
        m_oldYpos = p_y;
    }
    else
    {
        m_followTheMouse = false;
    }
}

void Controlers::mouseFollower(int p_newXpos, int p_newYpos)
{
    if(!m_followTheMouse)
        return;
    int p_deltaX = p_newXpos - m_oldXpos;
    int p_deltaY = p_newYpos - m_oldYpos;
    m_XAngle =  5.0f*p_deltaX/m_screenWidth;
    m_YAngle =  5.0f*p_deltaY/m_screenHeight;
    m_oldXpos = p_newXpos;
    m_oldYpos = p_newYpos;
}

void Controlers::onResize(int p_width, int p_height)
{
    m_screenWidth = p_width;
    m_screenHeight = p_height;
    glViewport(0, 0, p_width, p_height);
}

void Controlers::setModel(ModelController* p_modelController)
{
    m_modelController = p_modelController;
}
