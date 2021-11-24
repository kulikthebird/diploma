/**
 *    RegexConstraint.cpp
 *    Purpose:    
 *         File containts implementation of regex constraint algorithms.
 *
 *    @author Tomasz Kulik
*/


#include "../Inc/RegexConstraintClass.hpp"
#include "../Inc/RegexConstraint.hpp"
#include <vector>
#include <algorithm>
#include <set>


RegexConstraintI::RegexConstraintI(StatesVector states,
                 std::vector<StatesVector> feasibleValues,
                 std::vector<IloIntVar> vars)
        :  IloPropagatorI(vars[0].getEnv()),
        states(states),
        feasibleValues(feasibleValues),
        vars(vars)
{
    for(int i=0; i<vars.size(); i++)
        addVar(vars[i]);
}

RegexConstraintI::RegexConstraintI(std::vector<IloIntVar> vars, std::string str)
        : IloPropagatorI(vars[0].getEnv()),
          vars(vars)
{
    for(int i=0; i<vars.size(); i++)
        addVar(vars[i]);
    parseRegex(str, 0);
    addState({});    //Finish state. Every state with epsilon-path to this state is also a finishing state.
}

int RegexConstraintI::parseRegex(const std::string& str, uint index)
{
    StateNumber prelastState = states.size()-1;
    int currentNumber = 0;
    for(uint i=index; i<str.length(); i++)
        switch(str[i])
        {
            case '(':
                prelastState = states.size();
                i = parseRegex(str, i+1);
            break;
            case ')':
                addState({Transition {-1, (StateNumber) states.size()+1, true}});
                return i;
            break;
            case '*':
                states[states.size()-1][0].newState = prelastState;
                states[prelastState].push_back(Transition {-1, (StateNumber) states.size(), true});
            break;
            case '?':
                states[prelastState].push_back(Transition {-1, (StateNumber) states.size(), true});
            break;
            case '|':
                while(i<str.length()-1 and str[i+1] == ' ')
                    i++;
                if(str[i+1] != '(')
                {
                    states[prelastState].push_back(Transition {-1, (StateNumber) states.size()+1, true});
                    addState({Transition {-1, (StateNumber) states.size()+2, true}});
                }
                else
                {
                    states[prelastState].push_back(Transition {-1, (StateNumber) states.size()+1, true});
                    addState({});
                    prelastState = states.size()-1;
                    i = parseRegex(str, i+2);
                    states[prelastState].push_back(Transition {-1, (StateNumber) states.size(), true});
                }
            break;
            default:
                if(str[i] >= '0' and str[i] <= '9')
                {
                    currentNumber = currentNumber*10 + (str[i] - '0');
                    if(i>=str.length()-1 or str[i+1] < '0' or str[i+1] > '9')
                    {
                        prelastState = states.size();
                        addState({ Transition {currentNumber, (StateNumber) states.size()+1, false} });
                        currentNumber = 0;
                    }
                }
        }
    return str.length();
}

void RegexConstraintI::resetFeasibleValuesVectors()
{
    for(uint i=0; i<feasibleValues.size(); i++)
        for(uint j=0; j<feasibleValues[i].size(); j++)
            feasibleValues[i][j].clear();
}

void RegexConstraintI::addState(State&& state)
{
    states.push_back(state);
    feasibleValues.push_back(StatesVector(vars.size(), State()));
}

void RegexConstraintI::execute()
{
    resetFeasibleValuesVectors();
    std::set<StateNumber> statesAfterSearching = evaluateAutomatonWithInputVars();
    std::set<StateNumber> finishStates = findFinishStatesAfterSearch(statesAfterSearching);
    if(finishStates.empty())
    {
        violate();
        return;
    }
    reduceVarsDomains(finishStates);
}

std::set<StateNumber> RegexConstraintI::evaluateAutomatonWithInputVars()
{
    std::set<StateNumber> currentStates {0};
    for(uint i=0; i<vars.size() and not currentStates.empty(); i++)
    {
        std::set<StateNumber> nextStates;
        for(StateNumber state : currentStates)
        {
            std::vector<StateNumber> epsilonTransitionsState {state};
            std::vector<bool> visitedStates(states.size(), false);
            visitedStates[state] = true;
            for(int j=0; j<epsilonTransitionsState.size(); j++)
                for(auto& s : states[epsilonTransitionsState[j]])
                    if(s.newState >= (StateNumber)states.size())
                        continue;
                    else if(s.isEmptyString)
                    {
                        if(not visitedStates[s.newState])
                        {
                            visitedStates[s.newState] = true;
                            epsilonTransitionsState.push_back(s.newState);
                        }
                    }
                    else if(isInDomain(vars[i], s.value))
                    {
                        nextStates.insert(s.newState);
                        feasibleValues[s.newState][i].push_back({s.value, state, false});
                    }
        }
        std::swap(currentStates, nextStates);
    }
    return currentStates;
}

std::set<StateNumber> RegexConstraintI::findFinishStatesAfterSearch(std::set<StateNumber>& statesAfterSearching)
{
    std::set<StateNumber> finishStates;
    for(StateNumber state : statesAfterSearching)
    {
        if(state == states.size()-1)
            finishStates.insert(state);
        else
        {
            std::vector<StateNumber> epsilonTransitionsState {state};
            std::vector<bool> visitedStates(states.size(), false);
            visitedStates[state] = true;
            for(int j=0; j<epsilonTransitionsState.size(); j++)
                for(auto& s : states[epsilonTransitionsState[j]])
                    if(s.newState >= (StateNumber)states.size()-1 and s.isEmptyString)
                        finishStates.insert(state);
                    else if(s.isEmptyString and not visitedStates[s.newState])
                    {
                        visitedStates[s.newState] = true;
                        epsilonTransitionsState.push_back(s.newState);
                    }
        }
    }
    return finishStates;
}

void RegexConstraintI::reduceVarsDomains(std::set<StateNumber>& finishStates)
{
    for(int i=vars.size()-1; i>=0; i--)
    {
        std::set<StateNumber> nextStates;
        std::set<int> goodValues;
        for(uint state : finishStates)
            for(Transition srcState : feasibleValues[state][i])
            {
                nextStates.insert(srcState.newState);
                goodValues.insert(srcState.value);
            }
        auto min = *goodValues.begin();
        auto max = *goodValues.rbegin();
        if(vars[i].getLb() < min or vars[i].getUb() > max)
        {
            setMin(vars[i], min);
            setMax(vars[i], max);
        }
        else
            for(auto it = iterator(vars[i]); it.ok(); ++it )
                if(goodValues.find(*it) == goodValues.end())
                    removeValue(vars[i], *it);
        std::swap(finishStates, nextStates);
    }
}

IloPropagatorI* RegexConstraintI::makeClone(IloEnv env) const
{
    return new (env) RegexConstraintI(states, feasibleValues, vars);
}

IloConstraint RegexConstraint(std::vector<IloIntVar> vars, std::string str)
{
    return IloCustomConstraint(vars[0].getEnv(), new (vars[0].getEnv()) RegexConstraintI(vars, str));
}
