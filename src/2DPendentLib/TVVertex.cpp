#include "pch.h"
#include "TVVertex.h"

void TVVertex::VAO()
{
	glGenVertexArrays(1, &m_gVertexArrayObject);
	glBindVertexArray(m_gVertexArrayObject);

}

void TVVertex::VBO()
{
	glGenBuffers(1, &m_gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_gVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, m_vertexPosition.size() * sizeof(GLfloat), m_vertexPosition.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void TVVertex::VertexSpecification()
{
	VAO();
	VBO();
}

GLuint TVVertex::CompileShader(GLuint p_type, const std::string& p_source)
{
	GLuint shaderObject;

	if (p_type == GL_VERTEX_SHADER)
	{
		shaderObject = glCreateShader(GL_VERTEX_SHADER);

	}
	else if (p_type == GL_FRAGMENT_SHADER)
	{
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}

	const char* src = p_source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	return shaderObject;
}

GLuint TVVertex::CreateShaderProgram(const std::string& p_vertexShadersource, const std::string& p_fragmentShadersource)
{
	GLuint programObject = glCreateProgram();

	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, p_vertexShadersource);
	GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShadersource);

	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);

	glValidateProgram(programObject);

	return programObject;

}

void TVVertex::CreateGraphicsPipeline()
{
	m_gGraphicsPipelineShaderProgram = CreateShaderProgram("../../src/2DPendentLib/Shaders/TutoVideo.vert", "../../src/2DPendentLib/Shaders/TutoVideo.frag");

}
