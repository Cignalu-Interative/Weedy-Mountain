#include "pch.h"
#include "CameraOpenGL.h"
#include "InputManager.h"
#include "Vector2.h"
#include "Window.h"

CameraOpenGL::CameraOpenGL()
{
	m_Eye = glm::vec3(0.0f,0.0f,0.0f);

	m_ViewDirection = glm::vec3(0.0f,0.0f,-1.0f);

	m_UpVector = glm::vec3(0.0f,1.0f,0.0f);
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
