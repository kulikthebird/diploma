#include <Combiner.hpp>

void Combiner::runFirstPhase()
{
    FirstPhaseAlgorithm firstPhase;
    firstPhase.setModelElements(m_elements);
    firstPhase.setJointsSet(m_jointsList);
    firstPhase.runFirstPhase();
    m_resultStateCombination = firstPhase.getResultStateCombination();
}

void Combiner::runSecondPhase()
{
    SecondPhaseAlgorithm secondPhase;
    secondPhase.setModelElements(m_elements);
    secondPhase.setJointsSet(m_jointsList);
    secondPhase.setResultStateVector(m_resultStateCombination);
    secondPhase.runSecondPhase();
    m_moves = secondPhase.getMoves();
}

VMoves Combiner::getResultMoves()
{
    return m_moves;
}

VJointsList Combiner::getJointsSet()
{
    return m_jointsList;
}

VElements Combiner::getElements()
{
    return m_elements;
}

void Combiner::setElements(VElements p_elements)
{
    m_elements = p_elements;
}

void Combiner::setJointSet(VJointsList p_jointsList)
{
    m_jointsList = p_jointsList;
}
