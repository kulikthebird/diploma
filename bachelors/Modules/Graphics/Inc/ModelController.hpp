#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H


#include <ModelState.hpp>
#include <ModelView.hpp>

class ModelController
{
public:
    ModelController(VMoves& p_moves, VJointsList& p_jointsList, VElements& p_modelElements);
    void renderModel(glm::mat4& p_mvp, uint p_currentTime);
    void nextStep();
    void stepBack();

private:
    ModelView* m_modelView;
    ModelState* m_modelState;
};

#endif // MODELCONTROLLER_H
