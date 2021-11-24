#include <ModelController.hpp>


ModelController::ModelController(VMoves& p_moves, VJointsList& p_jointsList, VElements& p_modelElements)
{
    m_modelState = new ModelState(p_moves.size());
    m_modelView = new ModelView(m_modelState, p_moves, p_jointsList, p_modelElements);
}

void ModelController::renderModel(glm::mat4& p_mvp, uint p_currentTime)
{
    m_modelView->renderModel(p_mvp, p_currentTime);
}

void ModelController::nextStep()
{
    m_modelState->nextStep();
}

void ModelController::stepBack()
{
    m_modelState->stepBack();
}
