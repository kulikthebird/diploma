#include <FirstPhaseAlgorithm.hpp>
#include <SecondPhaseAlgorithm.hpp>


class Combiner
{
public:
    void runFirstPhase();
    void runSecondPhase();

    VJointsList getJointsSet();
    VElements getElements();
    VMoves getResultMoves();
    void setElements(VElements p_elements);
    void setJointSet(VJointsList p_jointsList);

private:
    VResultStateCombination m_resultStateCombination;
    VMoves m_moves;
    VElements m_elements;
    VJointsList m_jointsList;
};
