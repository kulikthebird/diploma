#pragma once
#include <BasicStructures.hpp>
#include <cmath>


template <typename T> int sgn(T val)
{
    return (T(0) <= val) - (val < T(0));
}

namespace Geometry
{

extern SAxis getJointAxis(VElements& p_elements, uint p_joint, VJointsList& p_jointsList);
extern void rotatePartOfModel(VElements& p_elements, uint p_joint, float p_rotateValue, VJointsList& p_jointsList);
extern Point rotateElementViaBasicAxis(Point p_pointToRotate, SAxis p_axis, float p_angle);
extern float cosinusOfAngleBetweenVectors(Point p_v1, Point p_v2);
extern bool checkThatPointLaysOnThePlane(Point p_axis, Point p_pointOnPlane, Point p_pointToCheck);
extern Point pointOnAxisProjection(Point p_point, Point p_axis);
extern Point crossProductVectors(Point p_v1, Point p_v2);
extern float angleBetweenTwoVectors(Point p_v1, Point p_v2, SAxis p_axis);

};
