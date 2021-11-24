#include <BasicStructures.hpp>
#include <fstream>


class Model
{
public:
    bool generateModelFromFile(const char* p_inputFilePath);
    void createModelElements(std::vector<uint> p_model);
    VElements getElementsVector();
    VJointsList getJointsList();

private:
    VElements m_elements;
    VJointsList m_jointsList;
};
