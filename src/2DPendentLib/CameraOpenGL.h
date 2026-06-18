#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
//#include "glm/ext/vector_relational.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "Vector2.h"


class CameraOpenGL
{
private:

	glm::mat4 m_ProjectionMatrix;

	glm::vec3 m_Eye;
	glm::vec3 m_ViewDirection;
	glm::vec3 m_UpVector;

	glm::vec2 m_OldMousePosition;

	glm::mat4 m_ViewMatrix;


public:

	CameraOpenGL();

	glm::mat4 GetViewMatrix() const;

	void SetProjectionMatrix(float p_fovy, float p_aspect, float p_near, float p_far);
	glm::mat4 GetProjectionMatrix() const;

	void MouseLook();
	void MoveForward(float p_speed);
	void MoveBackward(float p_speed);
	void MoveLeft(float p_speed);
	void MoveRight(float p_speed);

};

