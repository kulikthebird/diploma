#include <ModelView.hpp>
#include <Geometry.hpp>
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>


ModelView::ModelView(ModelState* p_modelState, VMoves& p_moves, VJointsList& p_jointsList, VElements& p_modelElements) :
    m_modelState(p_modelState), m_moves(p_moves), m_jointsList(p_jointsList), m_modelElements(p_modelElements)
{
    m_jointsAngles = std::vector<float>(m_jointsList.size(), 0.0f);
}

void ModelView::renderModel(glm::mat4 p_mvp, uint p_currentTime)
{
    animateJointRotation(p_currentTime);
    animateModelCentering(p_currentTime);
    glm::mat4 l_currentJointRotation(1.0f);
    glm::vec4 l_averagePoint(0.0f);
    const int l_middleElement = m_jointsList.size()/2;
    l_averagePoint += drawCube(p_mvp * glm::translate(glm::mat4(1.0f), -m_currentCenterPosition), l_currentJointRotation, m_jointsList[l_middleElement]);
    for(int i=l_middleElement; i<m_jointsList.size()-1; i++)
    {
        auto l_axis = Geometry::getJointAxis(m_modelElements, i, m_jointsList);
        l_currentJointRotation *= rotateViaJoint(m_jointsAngles[i], l_axis);
        for(int j=m_jointsList[i]+1; j<=m_jointsList[i+1]; j++)
            l_averagePoint += drawCube(p_mvp * glm::translate(glm::mat4(1.0f), -m_currentCenterPosition), l_currentJointRotation, j);
    }
    l_currentJointRotation = glm::mat4(1.0f);
    for(int i=l_middleElement-1; i>=0; i--)
    {
        auto l_axis = Geometry::getJointAxis(m_modelElements, i, m_jointsList);
        l_currentJointRotation *= rotateViaJoint(-m_jointsAngles[i], l_axis);
        for(int j=m_jointsList[i+1]-1; j>=(int)m_jointsList[i]; j--)
            l_averagePoint += drawCube(p_mvp * glm::translate(glm::mat4(1.0f), -m_currentCenterPosition), l_currentJointRotation, j);
    }
    m_targetCenterPosition = glm::vec3(l_averagePoint) * (1.0f / (m_modelElements.size()));
}

glm::vec4 ModelView::drawCube(glm::mat4&& p_centeredView, glm::mat4& p_currentJointRotation, uint p_elementNumber)
{
    Point l_position = m_modelElements[p_elementNumber].cord;
    glm::mat4 l_currentPosition = glm::translate(p_currentJointRotation, glm::vec3(l_position.x, l_position.y, l_position.z));
    auto l_worldCubePosition = p_centeredView * l_currentPosition * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    m_cube.drawCube(p_elementNumber % 2, l_worldCubePosition);
    return l_currentPosition * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 ModelView::rotateViaJoint(float p_angle, SAxis p_axis)
{
    glm::vec3 l_translatePoint = glm::vec3(p_axis.touchdown.x, p_axis.touchdown.y, p_axis.touchdown.z);
    glm::vec3 l_rotationAxis = glm::normalize(glm::vec3(p_axis.vector.x, p_axis.vector.y, p_axis.vector.z));
    glm::mat4 l_translate = glm::translate(glm::mat4(1.0f), -l_translatePoint);
    glm::mat4 l_reverseTranslate = glm::translate(glm::mat4(1.0f), l_translatePoint);
    return l_reverseTranslate * glm::toMat4(glm::angleAxis(p_angle, l_rotationAxis)) * l_translate;
}

void ModelView::animateJointRotation(uint p_currentTime)
{
    if(p_currentTime < m_targetAnimationTime)
        m_jointsAngles[m_rotatingJoint] =
                (p_currentTime - m_targetAnimationTime + m_animationDuration) *
                (m_targetJointRotationAngle - m_startJointRotationAngle) / m_animationDuration +
                m_startJointRotationAngle;
    else
    {
        m_jointsAngles[m_rotatingJoint] = m_targetJointRotationAngle;
        if(m_modelState->m_currentStep > m_currentViewStep)
            nextStep(p_currentTime);
        else if(m_modelState->m_currentStep < m_currentViewStep)
            stepBack(p_currentTime);
    }
}

void ModelView::nextStep(uint p_currentTime)
{
    prepareRotationAnimation(p_currentTime, false);
    m_currentViewStep++;
}

void ModelView::stepBack(uint p_currentTime)
{
    m_currentViewStep--;
    prepareRotationAnimation(p_currentTime, true);
}

void ModelView::prepareRotationAnimation(uint p_currentTime, bool p_reverseMove)
{
    m_renderFromBack = !m_renderFromBack;
    m_rotatingJoint = m_moves[m_currentViewStep].joint;
    m_targetAnimationTime = p_currentTime + m_animationDuration;
    m_startJointRotationAngle = m_jointsAngles[m_rotatingJoint];
    m_targetJointRotationAngle = m_startJointRotationAngle + (p_reverseMove? -1.0f : 1.0f) * m_moves[m_currentViewStep].move * M_PI/2;
}

void ModelView::animateModelCentering(uint p_currentTime)
{
    glm::vec3 l_deltaVector = m_targetCenterPosition - m_currentCenterPosition;
    float l_deltaVectorLen = glm::length2(l_deltaVector);
    float l_s = (p_currentTime - m_lastCenteringTime) * m_centeringSpeed;
    if(l_deltaVectorLen > 0.0f)
        m_currentCenterPosition += glm::normalize(l_deltaVector) * std::min(l_s, l_deltaVectorLen);
    m_lastCenteringTime = p_currentTime;
}
