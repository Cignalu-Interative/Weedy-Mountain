#include "pch.h"
#include "TVVertex.h"
#include "stb_image.h"

void TVVertex::VAO()
{
	glGenVertexArrays(1, &m_gVertexArrayObject);
	glBindVertexArray(m_gVertexArrayObject);

}

void TVVertex::VBO()
{
	//Position (location = 0)
	//Color (location = 1)

	glGenBuffers(1, &m_gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_gVertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(GLfloat), m_vertexData.data(), GL_STATIC_DRAW);

	const std::vector<GLuint> indexBufferData{ 2,0,1, 3,2,1 };

	glGenBuffers(1, &m_gIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gIndexBufferObject);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));


	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../res/2DPendentEngine/Assets/Textures/Other/Perceval.png", &width, &height, &nrChannels, 4);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char* src = p_source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	GLint compiled;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		// Récupère le message d'erreur
		GLint logLength;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<char> errorLog(logLength);
		glGetShaderInfoLog(shaderObject, logLength, NULL, errorLog.data());

		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
			<< errorLog.data() << std::endl;
	}

	return shaderObject;
}

GLuint TVVertex::CreateShaderProgram(const std::string& p_vertexShadersource, const std::string& p_fragmentShadersource)
{
	GLuint programObject = glCreateProgram();
	if (programObject == 0) {
		// Échec de création
		std::cerr << "erreur glCreateProgram" << std::endl;
		return 0;
	}

	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, p_vertexShadersource);
	GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShadersource);

	if (myVertexShader == 0 || myFragmentShader == 0) {
		std::cerr << "Échec critique : Un des shaders n'a pas pu être compilé. Impossible de lier le programme." << std::endl;
		return 0;
	}

	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);

	int linked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		// Récupère la longueur du log
		int logLength;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &logLength);

		// Récupère le log réel
		std::vector<char> errorLog(logLength);
		glGetProgramInfoLog(programObject, logLength, NULL, errorLog.data());

		// AFFICHE L'ERREUR DÉTAILLÉE (c'est CRUCIAL!)
		std::cerr << "=== PROGRAM LINKING FAILED ===\n";
		std::cerr << errorLog.data() << std::endl;
		std::cerr << "=== END OF ERROR ===\n";

		glDeleteShader(myVertexShader);
		glDeleteShader(myFragmentShader);
		glDeleteProgram(programObject);
		return 0;
	}

	glDetachShader(programObject, myVertexShader);
	glDetachShader(programObject, myFragmentShader);
	glDeleteShader(myVertexShader);
	glDeleteShader(myFragmentShader);

	return programObject;

}

void TVVertex::CreateGraphicsPipeline()
{
	std::string vertexSource = LoadShaderSourceFromFile("../../src/2DPendentLib/Shaders/TutoVideo.vert");
	std::string fragmentSource = LoadShaderSourceFromFile("../../src/2DPendentLib/Shaders/TutoVideo.frag");

	if (vertexSource.empty() || fragmentSource.empty())
	{
		std::cerr << "Erreur critique : Impossible de charger le code des shaders." << std::endl;
		return;
	}

	m_gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexSource, fragmentSource);

}

std::string TVVertex::LoadShaderSourceFromFile(const std::string& p_filePath)
{
	std::ifstream shaderFile(p_filePath);
	if (!shaderFile.is_open())
	{
		std::cerr << "ERREUR : Impossible d'ouvrir le fichier shader : " << p_filePath << std::endl;
		return "";
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	return shaderStream.str();
}

void TVVertex::GLCheck(const std::function<void()>& glFunction)
{
	GLClearAllErrors();

	glFunction();

	GLCheckErrorStatus("Fonction Lambda de GLCheck", 0);
}

void TVVertex::GLClearAllErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

bool TVVertex::GLCheckErrorStatus(const char* function, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "OpenGL Error:" << error
				  << "\tLine: " << __LINE__ 
			      << "\tfunction: " << function << std::endl;
		return true;
	}
	return false;
}

//int TVVertex::FindUniformLocation(GLuint p_pipeline, const GLchar* p_name)
//{
//	GLint location = glGetUniformLocation(p_pipeline, p_name);
//	if (location < 0)
//	{
//		std::cerr << "Could not find " << p_name << std::endl;
//		return -67;
//	}
//	return location;
//}
