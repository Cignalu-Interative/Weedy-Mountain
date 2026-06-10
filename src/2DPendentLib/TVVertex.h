#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"

class TVVertex
{
private:

	const std::vector<GLfloat> m_vertexPosition
	{
	//   x      y     z
		-0.8f, -0.8f, 0.0f, // vertex 1
		0.8f, -0.8f, 0.0f, // vertex 2
		0.0f, 0.8f, 0.0f // vertex 3
	};

	// VAO
	GLuint m_gVertexArrayObject = 0;

	// VBO
	GLuint m_gVertexBufferObject = 0;

	// GraphicsPipeline
	GLuint m_gGraphicsPipelineShaderProgram = 0;

public:

	void VAO();

	void VBO();

	void VertexSpecification();

	GLuint CompileShader(GLuint p_type, const std::string& p_source);

	GLuint CreateShaderProgram(const std::string& p_vertexShadersource, const std::string& p_fragmentShadersource);

	void CreateGraphicsPipeline();

	GLuint GetgGraphicsPipelineShaderProgram() { return m_gGraphicsPipelineShaderProgram; };


};

