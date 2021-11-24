#include <BasicStructures.hpp>
#include <Geometry.hpp>
#include <iostream>

using namespace std;

class FirstPhaseAlgorithm
{
public:
    void setJointsSet(VJointsList p_jointList);
    void setModelElements(VElements p_elementsPositionList);
    VResultStateCombination getResultStateCombination();
    void runFirstPhase();

private:
    bool checkThatCollisionsNotOccure(VElements &p_elements, uint p_joint);
    bool checkThatCurrentModelPartFitRequirement(VElements &p_input, uint p_joint, SCubeBounds p_inCube, SCubeBounds &p_outCube);
    bool searchForResultState(VElements p_input, uint p_joint, SCubeBounds p_inCube);
    bool checkThatCollisionsNotOccure(VElements &p_elements, uint p_joint, VJointsList &p_jointsList);

    VResultStateCombination m_combinationVector;
    uint m_modelSize;
    VElements m_elementsPositionList;
    VJointsList m_jointsList;
    float m_cubeSize;
};
