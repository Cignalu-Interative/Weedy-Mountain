#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
//#include "glm/ext/vector_relational.hpp"
#include "glm/gtx/rotate_vector.hpp"


class CameraOpenGL
{
private:

	glm::vec3 m_Eye;
	glm::vec3 m_ViewDirection;
	glm::vec3 m_UpVector;

	glm::vec2 m_OldMousePosition;

	glm::mat4 m_ViewMatrix;

public:

	CameraOpenGL();

	glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Eye, m_Eye + m_ViewDirection, m_UpVector); };

	void MouseLook();
	void MoveForward(float p_speed);
	void MoveBackward(float p_speed);
	void MoveLeft(float p_speed);
	void MoveRight(float p_speed);

};

