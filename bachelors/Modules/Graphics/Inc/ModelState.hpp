#ifndef MODELSTATE_H
#define MODELSTATE_H

#include <BasicStructures.hpp>


class ModelState
{
public:
    ModelState(uint p_numberOfMoves);

    void nextStep();
    void stepBack();

    uint m_currentStep = 0;

private:
    uint m_numberOfMoves;
};

#endif // MODELSTATE_H
