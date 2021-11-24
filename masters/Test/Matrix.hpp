#pragma once
#include <vector>
#include <initializer_list>


template <typename T>
struct Matrix
{
public:
    Matrix()
    {}

    template <typename ... Args>
    Matrix(unsigned int x, unsigned int y, Args... Fargs) :
         m_x(x), m_y(y)
    {
        m_mat.reserve(x*y);
        for(int i=0; i<x*y; i++)
            m_mat.push_back(T(Fargs...));
    }

    T& operator()(unsigned int p_index1, unsigned int p_index2)
    {
        if(p_index1 >= m_x || p_index2 >= m_y)
            throw OutOfRangeException();
        return m_mat[p_index1 + m_x*p_index2];
    }

    const T& operator()(unsigned int p_index1, unsigned int p_index2) const
    {
        return static_cast<const T&>(*this)(p_index1, p_index2);
    }

    std::vector<T> getColumn(unsigned int p_column)
    {
        std::vector<T> l_col(m_y);
        for(int y=0; y<m_y; y++)
            l_col[y] = (*this)(p_column, y);
        return l_col;
    }

    std::vector<T> getRow(unsigned int p_row)
    {
        std::vector<T> l_row(m_x);
        for(int x=0; x<m_x; x++)
            l_row[x] = (*this)(x, p_row);
        return l_row;
    }

    std::vector<T> getAllElements()
    {
        return m_mat;
    }

    unsigned int getXsize()
    { return m_x; }

    unsigned int getYsize()
    { return m_y; }

    class OutOfRangeException
    {
        public:
            const char* what()
            {
                return "Indices out of scope";
            }
    };

private:
    std::vector<T> m_mat;
    unsigned int m_x;
    unsigned int m_y;
};
