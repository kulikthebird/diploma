/**
 *    RegexConstraint.hpp
 *    Purpose:    
 *         Provides a mechanism that allows to describe a constraint with regular expression.
 *         Given a vector of variables: X = (X1, X2, ... , Xn) and regex R = "Some regular expression"
 *         using RegexConstraint(X, R) one can impose the condition that sequence evaluated sequence X
 *         must be accepted by regex R.
 *
 *    @author Tomasz Kulik
*/

#pragma once
#include <ilcp/cpext.h>
#include <vector>


/*
 * Main function of the regex constraint algorithm. This belongs to the user interface and should be used by user
 * to impose the constrants described by regex on vector of variables.
 * 
 * @param vars  The vector of input variables which the constraint mechanism will working on.
 * @param regex  String containing regular expression.
 * @return  IloConstraint object used by IBM CP engine (e.g. "model.add(RegexConstraint(vars, "Regex"));" ).
 */
IloConstraint RegexConstraint(std::vector<IloIntVar> vars, std::string regex);

