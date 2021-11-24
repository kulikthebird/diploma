#include <ModelState.hpp>


ModelState::ModelState(uint p_numberOfMoves) : m_numberOfMoves(p_numberOfMoves)
{}

void ModelState::nextStep()
{
    if(m_currentStep < m_numberOfMoves)
        m_currentStep++;
}

void ModelState::stepBack()
{
    if(m_currentStep > 0)
        m_currentStep--;
}
