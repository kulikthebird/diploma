#include <Model.hpp>
#include <Graphics.hpp>
#include <Combiner.hpp>
#include <ctime>


int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        std::cout << "Please provide a path to the model file" << std::endl;
        return 1;
    }
    Model m;
    Combiner c;
    if(!m.generateModelFromFile(argv[1]))
        return 1;
    c.setElements(m.getElementsVector());
    c.setJointSet(m.getJointsList());
    auto l_time = clock();
    c.runFirstPhase();
    c.runSecondPhase();
    std::cout << "The model has been solved in " << (double)(clock() - l_time) / (double)(CLOCKS_PER_SEC)  << " seconds" <<std::endl;
    Graphics g;
    g.initGraphics(c.getResultMoves(), c.getJointsSet(), c.getElements());
    return 0;
}
