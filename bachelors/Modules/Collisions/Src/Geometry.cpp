#include "Geometry.hpp"


namespace Geometry
{

SAxis getJointAxis(VElements& p_elements, uint p_joint, VJointsList& p_jointsList)
{
    auto l_vec = p_elements[p_jointsList[p_joint]].cord - p_elements[p_jointsList[p_joint+1]].cord;
    auto l_touchdownPoint = p_elements[p_jointsList[p_joint]].cord;
    return {l_vec, l_touchdownPoint};
}

void rotatePartOfModel(VElements& p_elements, uint p_joint, float p_rotateValue, VJointsList& p_jointsList)
{
    auto l_axis = getJointAxis(p_elements, p_joint, p_jointsList);
    for(uint i=p_jointsList.back(); i>p_jointsList[p_joint]; i--)
        p_elements[i].cord = rotateElementViaBasicAxis(p_elements[i].cord, l_axis, p_rotateValue);
}

Point rotateElementViaBasicAxis(Point p_pointToRotate, SAxis p_axis, float p_angle)
{
    Point l_primPoint = p_pointToRotate - p_axis.touchdown;
    if(!((p_axis.vector.x != 0) ^ (p_axis.vector.y != 0) ^ (p_axis.vector.z != 0)))
        std::cout << "ERROR: ratateElementViaBasicAxis: Axis is not OX, OY nor OZ!" << std::endl;
    if(p_axis.vector.x < 0 || p_axis.vector.y < 0 || p_axis.vector.z < 0)
        p_angle = -p_angle;
    if(p_axis.vector.x != 0)
        l_primPoint = Point{l_primPoint.x, l_primPoint.y*std::cos(p_angle) - l_primPoint.z*std::sin(p_angle), l_primPoint.y*std::sin(p_angle) + l_primPoint.z*std::cos(p_angle)};
    else if (p_axis.vector.y != 0)
        l_primPoint = Point{l_primPoint.x*std::cos(p_angle) + l_primPoint.z*std::sin(p_angle), l_primPoint.y, -l_primPoint.x*std::sin(p_angle) + l_primPoint.z*std::cos(p_angle)};
    else if(p_axis.vector.z != 0)
        l_primPoint = Point{l_primPoint.x*std::cos(p_angle) - l_primPoint.y*std::sin(p_angle), l_primPoint.x*std::sin(p_angle) + l_primPoint.y*std::cos(p_angle), l_primPoint.z};
    l_primPoint = l_primPoint + p_axis.touchdown;
    return {std::round(l_primPoint.x), std::round(l_primPoint.y), std::round(l_primPoint.z)};
}

float cosinusOfAngleBetweenVectors(Point p_v1, Point p_v2)
{
    return p_v1 * p_v2 / (p_v1.length() * p_v2.length());
}

bool checkThatPointLaysOnThePlane(Point p_axis, Point p_pointOnPlane, Point p_pointToCheck)
{
    return std::abs(p_axis * (p_pointToCheck - p_pointOnPlane)) < 0.01;
}

Point pointOnAxisProjection(Point p_point, Point p_axis)
{
    return p_axis * (p_point * p_axis / (p_axis * p_axis));
}

Point crossProductVectors(Point p_v1, Point p_v2)
{
    return Point{p_v1.y*p_v2.z, p_v1.z*p_v2.x, p_v1.x*p_v2.y} - Point{p_v1.z*p_v2.y, p_v1.x*p_v2.z, p_v1.y*p_v2.x};
}

float angleBetweenTwoVectors(Point p_v1, Point p_v2, SAxis p_axis)
{
    Point l_cross = crossProductVectors(p_v1, p_v2);
    float l_angle = acos(cosinusOfAngleBetweenVectors(p_v1, p_v2));
    if(sgn(p_axis.vector.x) == sgn(l_cross.x) && sgn(p_axis.vector.y) == sgn(l_cross.y) && sgn(p_axis.vector.z) == sgn(l_cross.z))
        l_angle *= -1;
    return l_angle;
}

};
