#include <Model.hpp>


bool Model::generateModelFromFile(const char* p_inputFilePath)
{
    std::fstream l_inputFile(p_inputFilePath, std::ios_base::in);
    if(!l_inputFile.good())
    {
        std::cout << "Wrong file name" << std::endl;
        return false;
    }
    std::vector<uint> l_model;
    uint l_num;
    while (l_inputFile >> l_num)
    {
        l_model.push_back(l_num);
    }
    createModelElements(l_model);
    return true;
}

void Model::createModelElements(std::vector<uint> p_model)
{
    Point l_current = {0, 0, 0};
    bool side = 0;
    m_elements.push_back({l_current});
    m_jointsList.push_back(0);
    for(auto p : p_model)
    {
        for(uint i=0; i<p-1; i++)
        {
            if(side)
            {
                l_current.x++;
                m_elements.push_back({l_current});
            }
            else
            {
                l_current.z++;
                m_elements.push_back({l_current});
            }
        }
        m_jointsList.push_back(m_jointsList.back()+p-1);
        side = 1 - side;
    }
    std::cout << "Model size: " << m_elements.size() << std::endl;
}

VElements Model::getElementsVector()
{
    return m_elements;
}

VJointsList Model::getJointsList()
{
    return m_jointsList;
}
