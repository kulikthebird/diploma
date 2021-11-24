#pragma once

#include <SDL2/SDL.h>
#include <ModelController.hpp>

class Controlers
{
public:
    void onKeyboard(SDL_Scancode p_key);
    void onMouse(bool p_isPushed, int p_x, int p_y);
    void mouseFollower(int p_newXpos, int p_newYpos);
    void onResize(int p_width, int p_height);
    void setModel(ModelController* p_modelController);

    ModelController* m_modelController;
    unsigned int m_oldXpos;
    unsigned int m_oldYpos;
    float m_XAngle = 0.0;
    float m_YAngle = 0.0;
    bool m_followTheMouse = false;
    float m_distance = 15.0;
    unsigned int m_screenWidth = 600;
    unsigned int m_screenHeight = 600;
};
