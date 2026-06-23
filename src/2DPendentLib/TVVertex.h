#pragma once
#include "glad/glad.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <functional>

#include "CameraOpenGL.h"

class TVVertex
{
private:

	const std::vector<GLfloat> m_vertexData
	{
	//    x      y      r     g     b     a      u     v
		-0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,// vertex 1 left down vertex
		0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // vertex 2 right down vertex
		-0.5f, 0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f,// vertex 3 left top vertex
		0.5f, 0.5f,    1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f  // vertex 4 right top vertex
	};

	// VAO
	GLuint m_gVertexArrayObject = 0;

	// VBO
	GLuint m_gVertexBufferObject = 0;

	// IBO
	GLuint m_gIndexBufferObject = 0;

	// GraphicsPipeline
	GLuint m_gGraphicsPipelineShaderProgram = 0;

	float g_uOffset = -2.0f;
	float g_uRotate = 0.0f;
	float g_uScale = 0.5f;

	glm::vec3 m_translation{ 0.0f, 0.0f, -2.0f };

	CameraOpenGL g_CameraOpenGL;

	unsigned int m_textureID = 0;

public:

	void VAO();

	void VBO();

	void VertexSpecification();

	GLuint CompileShader(GLuint p_type, const std::string& p_source);

	GLuint CreateShaderProgram(const std::string& p_vertexShadersource, const std::string& p_fragmentShadersource);

	void CreateGraphicsPipeline();

	std::string LoadShaderSourceFromFile(const std::string& p_filePath);

	GLuint GetgVertexArrayObject() { return m_gVertexArrayObject; };

	GLuint GetgVertexBufferObject() { return m_gVertexBufferObject; };

	GLuint GetgGraphicsPipelineShaderProgram() { return m_gGraphicsPipelineShaderProgram; };

	void GLCheck(const std::function<void()>& glFunction);

	static void GLClearAllErrors();

	static bool GLCheckErrorStatus(const char* function, int line);

	void SetUOffsetGL(float p_newOffsetGL) { g_uOffset = p_newOffsetGL; };
	float GetUOffesetGL() { return g_uOffset; };

	void SetURotate(float p_rotate) { g_uRotate = p_rotate; };
	float GetURotate() { return g_uRotate; };

	void SetUScale(float p_scale) { g_uScale = p_scale; };
	float GetUScale() { return g_uScale; };

	CameraOpenGL* GetCameraOpenGL() { return &g_CameraOpenGL; };

	void SetTranslation(glm::vec3 p_translation) { m_translation = p_translation; };
	glm::vec3 GetTranslation() { return m_translation; };

	//int FindUniformLocation(GLuint p_pipeline, const GLchar* p_name);

	unsigned int GetTextureID() { return m_textureID; };
};