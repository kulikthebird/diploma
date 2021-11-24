#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <BasicStructures.hpp>
#include <Cube.hpp>
#include <ModelState.hpp>
#include <glm/glm.hpp>


class ModelView
{
public:
    ModelView(ModelState* p_modelState, VMoves& p_moves, VJointsList& p_jointsList, VElements& p_modelElements);
    void renderModel(glm::mat4 p_mvp, uint p_currentTime);

private:
    glm::mat4 rotateViaJoint(float p_angle, SAxis p_axis);
    void animateJointRotation(uint p_currentTime);
    void animateModelCentering(uint p_currentTime);
    glm::vec4 drawCube(glm::mat4 &&p_centeredView, glm::mat4 &p_currentJointRotation, uint p_elementNumber);
    void prepareRotationAnimation(uint p_currentTime);
    void nextStep(uint p_currentTime);
    void stepBack(uint p_currentTime);
    void prepareRotationAnimation(uint p_currentTime, bool p_reverseMove);

    Cube m_cube;

    std::vector<float> m_jointsAngles;
    uint m_rotatingJoint;
    uint m_targetAnimationTime;
    float m_startJointRotationAngle;
    float m_targetJointRotationAngle;

    uint m_animationDuration = 1000;
    uint m_currentViewStep = 0;
    uint m_lastCenteringTime = 0;
    float m_centeringSpeed = 0.03;
    glm::vec3 m_currentCenterPosition = glm::vec3(0.0f);
    glm::vec3 m_targetCenterPosition = glm::vec3(0.0f);

    ModelState* m_modelState;
    VMoves m_moves;
    VJointsList m_jointsList;
    VElements m_modelElements;

    bool m_renderFromBack = false;
};


#endif // MODELVIEW_H
