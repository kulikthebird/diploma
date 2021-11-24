#include <SecondPhaseAlgorithm.hpp>


float norm(float p_angle)
{
    return p_angle < 0 ? 2*M_PI + p_angle : p_angle;
}

VMoves SecondPhaseAlgorithm::getMoves()
{
    return m_moves;
}

void SecondPhaseAlgorithm::setJointsSet(VJointsList p_jointList)
{
    m_jointsList = p_jointList;
}

void SecondPhaseAlgorithm::setModelElements(VElements p_elementsPositionList)
{
    m_elementsPositionList = p_elementsPositionList;
}

void SecondPhaseAlgorithm::setResultStateVector(VResultStateCombination p_resultCombinationVector)
{
    m_resultCombinationVector = p_resultCombinationVector;
}

int SecondPhaseAlgorithm::inverseMove(int p_move)
{
    return sgn(p_move)*(abs(p_move) - 4) % 4;
}

bool SecondPhaseAlgorithm::checkForCollisionDuringPointRotation(Point p_firstPoint, VElements& p_input, uint p_joint, float p_angle, SAxis p_axis)
{
    Point l_pointOnTheRotationAxis = Geometry::pointOnAxisProjection(p_firstPoint, p_axis.vector);
    for(uint j=m_jointsList[p_joint]+1; j<=m_jointsList.back(); j++)
    {
        Point l_secondPoint = p_input[j].cord - p_axis.touchdown;
        if(Geometry::checkThatPointLaysOnThePlane(p_axis.vector, l_pointOnTheRotationAxis, l_secondPoint))
        {
            Point d1 = p_firstPoint - l_pointOnTheRotationAxis;
            Point d2 = l_secondPoint - l_pointOnTheRotationAxis;
            if(abs(d1.length() - d2.length()) < 0.442 &&
              (norm(Geometry::angleBetweenTwoVectors(d1, d2, p_axis)) - norm(p_angle)) * sgn(p_angle) <= 0)
                  return false;
        }
    }
    return true;
}

bool SecondPhaseAlgorithm::checkCollisionDuringRotation(VElements p_input, uint p_joint, SAxis p_axis, float p_angle)
{
    for(uint i=0; i<m_jointsList[p_joint]; i++)
        if(!checkForCollisionDuringPointRotation(p_input[i].cord - p_axis.touchdown, p_input, p_joint, p_angle, p_axis))
            return false;
    return true;
}

bool SecondPhaseAlgorithm::rotateElement(VElements& p_input, uint p_joint, float p_angle)
{
    if(!checkCollisionDuringRotation(p_input, p_joint, Geometry::getJointAxis(p_input, p_joint, m_jointsList), p_angle))
        return false;
    Geometry::rotatePartOfModel(p_input, p_joint, p_angle, m_jointsList);
    return true;
}

void SecondPhaseAlgorithm::addMove(VElements& p_input, uint p_joint, int p_rotationValue)
{
    if(p_rotationValue == 0)
        return;
    if(rotateElement(p_input, p_joint, m_angle*p_rotationValue))
        m_moves.push_back({p_joint, p_rotationValue});
    else if(rotateElement(p_input, p_joint, m_angle*inverseMove(p_rotationValue)))
        m_moves.push_back({p_joint, inverseMove(p_rotationValue)});
}

bool SecondPhaseAlgorithm::tryToRotate(VElements& p_input, uint p_joint, int p_rotationValue)
{
    if(p_rotationValue != 0 &&
       !rotateElement(p_input, p_joint, m_angle*p_rotationValue) &&
       !rotateElement(p_input, p_joint, m_angle*inverseMove(p_rotationValue)))
            return false;
    return true;
}

bool SecondPhaseAlgorithm::findMoveToAvoidCollision(VElements& p_input, VMoves& p_permutation, uint x)
{
    for(uint y=x+1; y<p_permutation.size(); y++)
    {
        std::swap(p_permutation[x], p_permutation[y]);
        if(tryToRotate(p_input, p_permutation[x].joint, p_permutation[x].move))
            return true;
    }
    return false;
}

bool SecondPhaseAlgorithm::permutateMovesList(VElements&& p_input, VMoves& p_permutation)
{
    for(uint x=0; x<p_permutation.size(); x++)
        if(!tryToRotate(p_input, p_permutation[x].joint, p_permutation[x].move) &&
            !findMoveToAvoidCollision(p_input, p_permutation, x))
            {
                std::swap(p_permutation[x], p_permutation[rand()%x]);
                return false;
            }
    return true;
}

bool SecondPhaseAlgorithm::searchForResultPath(VElements p_input, VMoves& p_permutation)
{
    while(!permutateMovesList(VElements(p_input), p_permutation));
    return true;
}

bool SecondPhaseAlgorithm::findMovesVector()
{
    VMoves permutation;
    for(uint i=1; i<m_resultCombinationVector.size()-2; i++)
        permutation.push_back({i, m_resultCombinationVector[i]});
    std::cout << "Permutation set size: " << permutation.size() << std::endl;
    if(searchForResultPath(m_elementsPositionList, permutation))
        std::cout << "Found moves permutation." << std::endl;
    for(SMove p : permutation)
        addMove(m_elementsPositionList, p.joint, p.move - m_currentCombinationVector[p.joint]);
    return false;
}

void SecondPhaseAlgorithm::runSecondPhase()
{
    std::cout << "Second phase is starting." << std::endl;
    m_modelSize = m_jointsList.size();
    m_currentCombinationVector = VResultStateCombination(m_modelSize, 0);
    findMovesVector();
}
