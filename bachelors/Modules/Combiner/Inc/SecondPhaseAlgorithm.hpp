#include <BasicStructures.hpp>
#include <Geometry.hpp>

#include <iostream>

using namespace std;


class SecondPhaseAlgorithm
{
public:
    void runSecondPhase();
    void setJointsSet(VJointsList p_jointList);
    void setModelElements(VElements p_elementsPositionList);
    void setResultStateVector(VResultStateCombination p_resultCombinationVector);

    bool checkCollisionDuringRotation(VElements p_input, uint p_joint, SAxis p_axis, float p_angle);
    bool rotateElement(VElements& p_input, uint p_joint, float p_angle);
    bool searchForResultPath(VElements p_input, VMoves &permutation);
    bool checkForCollisionDuringPointRotation(Point p_firstPoint, VElements& p_input, uint p_joint, float p_angle, SAxis p_axis);

    int inverseMove(int p_move);
    bool tryToRotate(VElements& p_input, uint p_joint, int p_rotationValue);
    void addMove(VElements& p_input, uint p_joint, int p_rotationValueb);
    bool findMovesVector();
    VMoves getMoves();

    VMoves m_moves;
    VResultStateCombination m_resultCombinationVector;
    VResultStateCombination m_currentCombinationVector;
    uint m_modelSize;
    VElements m_elementsPositionList;
    VJointsList m_jointsList;
    const float m_angle = M_PI/2;
private:
    bool findMoveToAvoidCollision(VElements& p_input, VMoves& p_permutation, uint x);
    bool permutateMovesList(VElements&& p_input, VMoves& p_permutation);
};
