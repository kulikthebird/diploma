/**
 *    RegexConstraintClass.hpp
 *    Purpose:    
 *          Main class of the regex constraint mechanism. It is used by IBM CP solver's engine.
 *          The class contains domain filtering algorithm based on NFA automaton compiling and
 *          evaluation.
 *
 *    @author Tomasz Kulik
*/


#include <ilcp/cpext.h>
#include <ilcp/cp.h>
#include <vector>
#include <set>


/*
 * Structures used by RegexConstraint
 */
typedef unsigned int StateNumber;
struct Transition
{
    int value;
    StateNumber newState;
    bool isEmptyString;
};
typedef std::vector<Transition> State;
typedef std::vector<State> StatesVector;


/**
 *  Class used by IBM CP solver's engine.
 */
class RegexConstraintI : public IloPropagatorI
{
public:

    /*
     * Constructor of regex constraint. It compiles regex to NFA and prepares feasibleValues vector.
     */
    RegexConstraintI(std::vector<IloIntVar> vars, std::string regex);

    /*
     * Constructor used by IBM CP engine.
     */
    RegexConstraintI(StatesVector states,
                     std::vector<StatesVector> rect,
                     std::vector<IloIntVar> vars);

    /*
     * Function used by IBM CP engine
     */
    IloPropagatorI* makeClone(IloEnv env) const override;

    /*
     * Method that is called during constraint propagation. The result of this method's work are reduced domains.
     */
    void execute() override;

private:
    void resetFeasibleValuesVectors();
    void addState(State&& state);
    int parseRegex(const std::string& str, uint index);
    std::set<StateNumber> evaluateAutomatonWithInputVars();
    std::set<StateNumber> findFinishStatesAfterSearch(std::set<StateNumber>& statesAfterSearching);
    void reduceVarsDomains(std::set<StateNumber>& finishStates);

    /*
     * A vector used to store states created during regex compilation.
     */
    StatesVector states;

    /*
     * A vector of feasible values filled during automaton evaluation. For every state and every input variable
     * the algorithm allocates a vector of states and values from wich given state was reached.
     */
    std::vector<StatesVector> feasibleValues;

    /*
     * A vector of IBM CP integer variables.
     */
    std::vector<IloIntVar> vars;
};

