#pragma once

#include <iostream>
#include <vector>
#include <cmath>


typedef unsigned int uint;

struct Point
{
    float x, y, z;
    Point operator+(const Point& r)
    {
        return {x + r.x, y + r.y, z + r.z};
    }
    Point operator-(const Point& r)
    {
        return {x - r.x, y - r.y, z - r.z};
    }
    float operator*(const Point& r)
    {
        return x * r.x + y * r.y + z * r.z;
    }
    template<class T>
    Point operator*(const T& r)
    {
        return {x * r, y * r, z * r};
    }
    bool operator==(const Point& r)
    {
        return x == r.x && y == r.y && z == r.z;
    }
    friend std::ostream& operator<<(std::ostream &os, Point const& p)
    {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
    float length()
    {
        return sqrt((*this) * (*this));
    }
};

struct SCubeBounds
{
    float minx, miny, minz;
    float maxx, maxy, maxz;

    void evaluate(const Point& p_point)
    {
        if(p_point.x < minx)
            minx = p_point.x;
        if(p_point.x > maxx)
            maxx = p_point.x;

        if(p_point.y < miny)
            miny = p_point.y;
        if(p_point.y > maxy)
            maxy = p_point.y;

        if(p_point.z < minz)
            minz = p_point.z;
        if(p_point.z > maxz)
            maxz = p_point.z;
    }

    float getMaxBound()
    {
        float l_max = 0.0f;
        float l_tempMax;
        l_tempMax = maxx - minx;
        if(l_tempMax > l_max)
            l_max = l_tempMax;
        l_tempMax = maxy - miny;
        if(l_tempMax > l_max)
            l_max = l_tempMax;
        l_tempMax = maxz - minz;
        if(l_tempMax > l_max)
            l_max = l_tempMax;
        return l_max;
    }
};

struct SElement
{
    Point cord;
};

struct SMove
{
    uint joint;
    int move;
};

struct SAxis
{
    Point vector;
    Point touchdown;
};

typedef std::vector<int> VResultStateCombination;
typedef std::vector<SElement> VElements;
typedef std::vector<uint> VJointsList;
typedef std::vector<SMove> VMoves;
