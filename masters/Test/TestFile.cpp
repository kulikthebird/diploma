#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <ilcp/cp.h>
#include <RegexConstraint.hpp>
#include <Matrix.hpp>
#include <Nonograms.hpp>


void testSolveNonogramUsingRegexConstraint(const Nonogram&);
void testRandomNonogramsGenerator();
void testSingleRegex();
void testWorstCase();
void testPropagator();


int main(int argc, char** argv)
{
	testSingleRegex();
	testPropagator();
    testSolveNonogramUsingRegexConstraint(dog);
    testSolveNonogramUsingRegexConstraint(bear);
    testSolveNonogramUsingRegexConstraint(coala);
    testSolveNonogramUsingRegexConstraint(drakkar);
    testWorstCase();
	testRandomNonogramsGenerator();
    return 0;
}


void printTimeResults(std::map<uint, std::vector<float>>& timeResults)
{
    for(auto const& resultBySize : timeResults)
    {
        std::cout << "Size " << resultBySize.first << "*" << resultBySize.first << ": ";
        for(auto result : resultBySize.second)
            std::cout << result << "sec., ";
        std::cout << std::endl;
    }
}


void testSolveNonogramUsingRegexConstraint(const Nonogram& img)
{
    std::cout << "\n\n ## Test of fixed image ##:\n\n";
    IloEnv env;
    try
    {
        IloModel model(env);
        auto rowSize = img.cols.size();
        auto colSize = img.rows.size();
        Matrix<IloIntVar> image(rowSize, colSize, env);
        for(int x=0; x<rowSize; x++)
            model.add(RegexConstraint(image.getColumn(x), generateRowRegex(img.cols[x])));
        for(int y=0; y<colSize; y++)
            model.add(RegexConstraint(image.getRow(y), generateRowRegex(img.rows[y])));
        IloCP solver(model);
        solver.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
        solver.solve();
        std::cout << "Time: " << solver.getInfo(IloCP::SolveTime) << " s." << std::endl;
        for(int y=0; y<colSize; y++)
        {
            for(int x=0; x<rowSize; x++)
                std::cout << ((solver.getValue(image(x, y)) == 1)?"#":" ");
            std::cout << std::endl;
        }
    } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
    env.end();
}


void testSingleRegex()
{
    std::cout << "\n\n ## Test of generating all possible sequences from regex ##:\n\n";
    IloEnv env;
    try
    {
        IloModel model(env);
        std::vector<IloIntVar> vars(6);
        for(auto& var : vars)
            var = IloIntVar(env);
        model.add(RegexConstraint(vars, "((1 0 0)|(1 0 1))*1?(0*22*)"));
        IloCP solver(model);
        solver.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
        solver.startNewSearch();
        while(solver.next())
        {
            for(auto& var : vars)
                std::cout << solver.getValue(var) << " ";
            std::cout << std::endl;
        }
    } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
    env.end();
}


void testWorstCase()
{
    std::cout << "\n\n ## Test of time complexity ##:\n\n";
    for(uint vectorLength = 10; vectorLength <= 100 ; vectorLength+=10)
    {
        const uint oneVariableDomainSpread = 700;
        const auto regex = generateComplexRegex(oneVariableDomainSpread);
        IloEnv env;
        try
        {
            IloModel model(env);
            std::vector<IloIntVar> vars(vectorLength);
            for(auto& var : vars)
                var = IloIntVar(env);
            model.add(RegexConstraint(vars, regex));
            IloCP solver(model);
            solver.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
            solver.propagate();
            std::cout << "Solving time for vector of length " << vectorLength << ": " << solver.getInfo(IloCP::SolveTime)
                       << " s." << std::endl;
        } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
        env.end();
    }
    for(uint oneVariableDomainSpread = 20; oneVariableDomainSpread <= 200 ; oneVariableDomainSpread+=20)
    {
        const uint vectorLength = 300;
        IloEnv env;
        try
        {
            IloModel model(env);
            std::vector<IloIntVar> vars(vectorLength);
            for(auto& var : vars)
                var = IloIntVar(env);
            model.add(RegexConstraint(vars, generateComplexRegex(oneVariableDomainSpread)));
            IloCP solver(model);
            solver.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
            solver.propagate();
            std::cout << "Solving time for regex complexity " << oneVariableDomainSpread << ": " << solver.getInfo(IloCP::SolveTime)
                        << "s." << std::endl;
        } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
        env.end();
    }
}


void testRandomNonogramsGenerator()
{
    std::cout << "\n\n ## Test of random generated images ##:\n\n";
    std::map< uint, std::vector<float> > timeResults;
    const auto maxTestSideLength = 40;
    for(uint size=10; size<=maxTestSideLength; size+=5)
    {
        const auto rowSize = size;
        const auto colSize = size;
        timeResults[size] = std::vector<float>();
        std::cout << "Test for images of side length: " << size << " / " << maxTestSideLength << 
                     ". It may take a while." <<std::endl;
        for(uint j=0; j<6; j++)
        {
            IloEnv env;
            try
            {
                Matrix<uint> randomImg = generateRandomImage(rowSize, colSize);
                Rows rows;
                Rows cols;
                for(int x=0; x<size; x++)
                    cols.push_back(calculateIntervals(randomImg.getColumn(x)));
                for(int y=0; y<colSize; y++)
                    rows.push_back(calculateIntervals(randomImg.getRow(y)));
                IloModel model(env);
                Matrix<IloIntVar> imageVars(rowSize, colSize, env);
                for(int x=0; x<rowSize; x++)
                    model.add(RegexConstraint(imageVars.getColumn(x), generateRowRegex(cols[x])));
                for(int y=0; y<colSize; y++)
                    model.add(RegexConstraint(imageVars.getRow(y), generateRowRegex(rows[y])));
                IloCP solver(model);
                solver.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
                solver.solve();
                timeResults[size].push_back(solver.getInfo(IloCP::SolveTime));
            }
            catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
            env.end();
        }
    }
    std::cout << "\n\n ## Time results of solving random nonograms ##:\n\n";
    printTimeResults(timeResults);
}


void testPropagator()
{
    std::cout << "\n\n ## Test of the propagator ##:";
    IloEnv env;
    std::cout << "\n\n ## Domains in the middle should be fixed to 1. ##:\n\n";
    try
    {
        IloModel model(env);
        std::vector<IloIntVar> vars(10);
        for(auto& var : vars)
            var = IloIntVar(env);
        model.add(RegexConstraint(vars, "0*1 1 1 1 1 1 1 0*"));
        IloCP solver(model);
        solver.propagate();
        for(auto& var : vars)
            std::cout << solver.domain(var) << " ";
        std::cout << std::endl;
    } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
    std::cout << "\n\n ## There is no solution (no propagation occures)##:\n\n";
    try
    {
        IloModel model(env);
        std::vector<IloIntVar> vars(10);
        for(auto& var : vars)
            var = IloIntVar(env, 0, 9);
        model.add(RegexConstraint(vars, "2 0*"));
        model.add(RegexConstraint(vars, "3 0*"));
        IloCP solver(model);
        solver.propagate();
        for(auto& var : vars)
            std::cout << solver.domain(var) << " ";
        std::cout << std::endl;
    } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
    std::cout << "\n\n ## Two regex constraints on one vector ##:\n\n";
    try
    {
        IloModel model(env);
        std::vector<IloIntVar> vars(10);
        for(auto& var : vars)
            var = IloIntVar(env, 0, 3);
        model.add(RegexConstraint(vars, "2 2 2 2 2 (0|2|3)*"));
        model.add(RegexConstraint(vars, "(0|2|3)* 0 2 2 3 3"));
        IloCP solver(model);
        solver.propagate();
        for(auto& var : vars)
            std::cout << solver.domain(var) << " ";
        std::cout << std::endl;
    } catch (IloException& ex) { std::cerr << "Error:" << ex << std::endl; }
    env.end();
}
