#include <vector>
#include <Matrix.hpp>


using Rows = std::vector<std::vector<unsigned int>>;
struct Nonogram
{
	Rows rows;
	Rows cols;
};

std::string generateComplexRegex(uint oneVariableDomainSpread)
{
    std::string result = "(0*";
	for(uint j=1; j<oneVariableDomainSpread; j++)
		result += " | (" + std::to_string(j) + ")*";
	result += ")*";
    return result;
}

Matrix<uint> generateRandomImage(uint rowSize, uint colSize)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Matrix<uint> randomImg(rowSize, colSize);
    for(int x=0; x<rowSize; x++)
        for(int y=0; y<colSize; y++)
            randomImg(x, y) = (dist(mt) >= 0.5)?1:0;
    return randomImg;
}

std::vector<uint> calculateIntervals(std::vector<uint> row)
{
    std::vector<uint> rowIntervals;
    int currentInterval = 0;
    for(auto value : row)
    {
        if(value == 1)
            currentInterval++;
        else if(currentInterval > 0)
        {
            rowIntervals.push_back(currentInterval);
            currentInterval = 0;
        }
    }
    if(currentInterval > 0)
        rowIntervals.push_back(currentInterval);
    if(rowIntervals.size() == 0)
        rowIntervals.push_back(0);
    return rowIntervals;
}

std::string generateRowRegex(std::vector<uint> input)
{
    std::string result;
    for(auto a : input)
    {
        result += " 0*";
        for(uint i=0; i<a; i++)
            result += " 1 ";
        result += " 0";
    }
    result += "*";
    return result;
}

template <class T>
IloIntVarArray vecToCpArray(std::vector<T>&& vec, IloEnv env)
{
    IloIntVarArray result(env, vec.size());
    for(int i=0; i<vec.size(); i++)
        result[i] = vec[i];
    return result;
}


Nonogram dog = 
{
	{{2}, {2,1}, {4,1}, {4}, {1,1}, {1,1}},
	{{2}, {2}, {1,4}, {4},{1},{4}}
};

Nonogram bear =
{
	{{3,3}, {3,3}, {7}, {1,1}, {1,1,1,2}, {1,1}, {1,2,1}, {2,1}, {2,2}, {4}},
	{{2,5}, {3,1}, {3,1}, {1,1,1}, {1,1}, {1,1,1}, {3,1,1,1}, {4,2}, {2,1,1}, {4}}
};

Nonogram coala =
{
	{
		{2,2,2},{3,1,7,1},{3,1,9,1},{4,13},{4,1,3,3},{4,2,5},{5,2,5},{4,2,5},
		{4,7},{5,7},{3,9},{2,7,6},{1,5,7},{2,9},{6,2,4},{5,6},{3,8},{3,4}, {3},{4}
	},
	{
		{5},{7},{9},{9},{9},{5,2},{1,2,2},{3,2,4},{1,1,2,5},{1,6,2,6},{3,4,3,2,2},
		{4,4,1,2,1},{4,4,2,2,1},{10,3,2},{3,9,3},{13,2},{16},{1,6,8},{1,1,7},{3,5}
	}
};

Nonogram drakkar =
{
	{
		{1}, {1},
		{21}, {2,18},
		{1,4,8}, {1,3,2,4},
		{1,2,2,5}, {2,2,1,4},
		{1,2,2,4}, {2,2,1,1},
		{1,2,2,3,1}, {2,1,1,2,2},
		{1,2,1,7}, {2,1,1,6},
		{1,1,1,3,2}, {2,2,1,6},
		{1,1,1,4,2}, {1,1,1,1,2},
		{2,1,1,1,2}, {1,1,1,1,2},
		{2,2,1,1,3}, {2,1,1,1,4},
		{2,1,1,2,5}, {2,1,2,1,6},
		{1,2,1,2,3,3}, {1,1,12,2,3,2},
		{1,1,4,3,3,1,1}, {3,1,3,1,1,3,1,1},
		{3,1,2,1,4,3,1}, {1,3,1,1,5,3,1,1},
		{2,8,1,5,3,1,1}, {9,6,4,4,1},
		{3,13,4,5}, {13,4,5,4},
		{2,2,4,4,5,2}, {2,2,4,9,5},
		{3,2,15,4,1}, {5,7,2},
		{3,16,2}, {3,2,2},
		{5,4,4,4,3,2,7}, {5,6,3,3},
		{7,3,8,2,3,3}, {14,4,1,5},
		{5,12,5,3}, {7,13,3,5},
		{12,4}, {4,7,7,3,9},
		{1,3}, {13,6,5,16,5}
	},
	{
		{2,1,1}, {2,1,1,1},
		{1,1,1,1,1,1,1}, {1,1,1,1,1,1,1},
		{2,1,1,1,1,1}, {3,1,1,1,1,1,1,1},
		{1,3,1,1,1,2,1,1}, {1,3,1,1,1,1,1,1,1},
		{1,3,5,1,1,2,1,1,1}, {1,3,9,1,1,2,1,1},
		{1,3,4,3,3,1,2,1,1}, {1,4,5,2,2,1,1,1},
		{1,6,3,2,2,1,1,1}, {1,13,2,1,1,1},
		{2,3,2,2,1,1,1}, {2,4,2,1,1,1,1,1},
		{2,1,3,2,2,1,1,1,1}, {3,2,1,1,1,1,1,1,1,1},
		{4,1,3,1,1,1,1,1,1,1}, {5,2,2,1,1,2,1,1,1,1},
		{7,1,2,1,1,1,1,3,1}, {4,4,7,1,1,1,1,2,1},
		{2,4,2,2,1,1,2,2,1,1}, {1,4,1,2,1,1,2,2,1,1},
		{1,6,2,1,1,1,1,1,2,1,1}, {1,6,2,1,1,1,1,2,1,1},
		{1,1,1,1,1,1,1,2,1,1}, {2,1,1,1,1,1,1,1,1,1},
		{2,1,2,1,2,1,1,1,2,1}, {3,1,1,1,2,1,2,1,1,1},
		{4,1,1,1,1,1,1,1,1,1}, {2,3,1,1,1,1,1,2,2,1},
		{1,3,1,2,1,1,1,1,1,2,1}, {1,4,1,1,1,1,1,1,1,3,1},
		{2,4,2,1,1,1,1,2,2,1,2,1}, {1,6,1,2,1,2,2,1,1,3,1},
		{2,4,1,2,1,3,1,1,1}, {2,1,1,1,1,1,1,2,1,1},
		{1,2,2,1,1,1,1,1,1,1,1}, {2,1,3,2,1,2,1,2,1,1,2,1},
		{3,3,3,3,1,1,2,1,1,1,1}, {7,4,3,1,1,1,2,1,1,1},
		{2,5,4,2,1,1,1,1,1,1,1}, {1,13,1,2,2,2,1,1,1,1},
		{13,2,1,1,3,1,1,1}, {4,1,5,1,1,1,4,1,1,1,1},
		{5,18,1,2,1}, {3,3,4,1,1,1,4},
		{5,1,1,1,1,1,2}, {1,1,1,1,1,2}
	}
};
