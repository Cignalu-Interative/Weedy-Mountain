#include "pch.h"
#include "CameraOpenGL.h"
#include "InputManager.h"
#include "Window.h"

CameraOpenGL::CameraOpenGL()
{
	m_Eye = glm::vec3(0.0f,0.0f,0.0f);

	m_ViewDirection = glm::vec3(0.0f,0.0f,-1.0f);

	m_UpVector = glm::vec3(0.0f,1.0f,0.0f);
}

glm::mat4 CameraOpenGL::GetViewMatrix() const
{
    return glm::lookAt(m_Eye, m_Eye + m_ViewDirection, m_UpVector);
}

void CameraOpenGL::SetProjectionMatrix(float p_fovy, float p_aspect, float p_near, float p_far)
{
    m_ProjectionMatrix = glm::perspective(p_fovy, p_aspect, p_near, p_far);
}

glm::mat4 CameraOpenGL::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

void CameraOpenGL::MouseLook()
{
    Vector2i mouseDeltaRaw = InputManager::Get()->GetRELMousePosition();

    float sensitivity = 0.2f;
    float deltaX = static_cast<float>(mouseDeltaRaw.GetX()) * sensitivity;
    float deltaY = static_cast<float>(mouseDeltaRaw.GetY()) * sensitivity;
    
    m_ViewDirection = glm::rotate(m_ViewDirection, glm::radians(-deltaX), m_UpVector);
}

void CameraOpenGL::MoveForward(float p_speed)
{
	m_Eye += (m_ViewDirection * p_speed);
}

void CameraOpenGL::MoveBackward(float p_speed)
{
	m_Eye -= (m_ViewDirection * p_speed);
}

void CameraOpenGL::MoveLeft(float p_speed)
{
    glm::vec3 rightVector = glm::cross(m_ViewDirection, m_UpVector);
    m_Eye -= rightVector * p_speed;
}

void CameraOpenGL::MoveRight(float p_speed)
{
    glm::vec3 rightVector = glm::cross(m_ViewDirection, m_UpVector);
    m_Eye += rightVector * p_speed;
}
