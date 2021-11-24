#include <FirstPhaseAlgorithm.hpp>
#include <iostream>


void FirstPhaseAlgorithm::setJointsSet(VJointsList p_jointList)
{
    m_jointsList = p_jointList;
}

void FirstPhaseAlgorithm::setModelElements(VElements p_elementsPositionList)
{
    m_elementsPositionList = p_elementsPositionList;
}

VResultStateCombination FirstPhaseAlgorithm::getResultStateCombination()
{
    return m_combinationVector;
}

void FirstPhaseAlgorithm::runFirstPhase()
{
    std::cout << "First phase is starting." << std::endl;
    m_modelSize = m_jointsList.size();
    m_combinationVector = VResultStateCombination(m_modelSize, 0);
    m_cubeSize = ceil(pow(m_elementsPositionList.size(), (double)1/3));
    std::cout << "Side length of a cube: " << m_cubeSize << std::endl;
    SCubeBounds p_initialCubeBound = {m_elementsPositionList[0].cord.x, m_elementsPositionList[0].cord.y, m_elementsPositionList[0].cord.z,
                                     m_elementsPositionList[0].cord.x, m_elementsPositionList[0].cord.y, m_elementsPositionList[0].cord.z};
    if(searchForResultState(m_elementsPositionList, 1, p_initialCubeBound))
    {
        m_combinationVector[m_combinationVector.size()-1] = 0;
        std::cout << "Result combination vector has been found." << std::endl;
    }
    else
        std::cout << "The combination vector has not been found :(" << std::endl;
}

bool FirstPhaseAlgorithm::checkThatCurrentModelPartFitRequirement(VElements& p_input, uint p_joint, SCubeBounds p_inCube, SCubeBounds& p_outCube)
{
    auto l_elementsToCheck = (p_joint+2 < m_jointsList.size())  ? m_jointsList[p_joint+2] : m_jointsList.back();
    for(uint i=m_jointsList[p_joint]; i<=l_elementsToCheck; i++)
        p_inCube.evaluate(p_input[i].cord);
    if(p_inCube.getMaxBound() >= m_cubeSize-0.5)
        return false;
    p_outCube = p_inCube;
    return true;
}

bool FirstPhaseAlgorithm::searchForResultState(VElements p_input, uint p_joint, SCubeBounds p_inCube)
{
    SCubeBounds l_outCube;
    for(int i=0; i<4; i++)
    {
        if(checkThatCollisionsNotOccure(p_input, p_joint, m_jointsList) &&
           checkThatCurrentModelPartFitRequirement(p_input, p_joint, p_inCube, l_outCube) &&
           (p_joint >= m_modelSize-2 ||
            searchForResultState(p_input, p_joint+1, l_outCube)))
               return true;
        Geometry::rotatePartOfModel(p_input, p_joint, -M_PI/2, m_jointsList);
        ++m_combinationVector[p_joint] %= 4;
    }
    return false;
}

bool FirstPhaseAlgorithm::checkThatCollisionsNotOccure(VElements &p_elements, uint p_joint, VJointsList &p_jointsList)
{
    const auto& l_elementsToCheckLower = p_jointsList[p_joint];
    const auto& l_elementsToCheckUpper = p_jointsList[std::min(p_joint + 2, (uint)p_jointsList.size() - 1)];
    for(uint i=0; i<l_elementsToCheckLower; i++)
        for(uint j=l_elementsToCheckLower; j<=l_elementsToCheckUpper; j++)
            if(p_elements[i].cord == p_elements[j].cord)
                return false;
    return true;
}
