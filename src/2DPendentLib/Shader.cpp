#include "pch.h"
#include "Shader.h"
#include <SDL.h>

Shader::~Shader()
{
    if (m_programID != 0)
        glDeleteProgram(m_programID);
}

bool Shader::LoadFromFiles(const std::string& p_vertPath,
    const std::string& p_fragPath)
{
    std::string vertSrc = ReadFile(p_vertPath);
    std::string fragSrc = ReadFile(p_fragPath);
    if (vertSrc.empty() || fragSrc.empty())
    {
        SDL_Log("[Shader] Impossible de lire les fichiers shader");
        return false;
    }
    return LoadFromSource(vertSrc.c_str(), fragSrc.c_str());
}

bool Shader::LoadFromSource(const char* p_vertSrc, const char* p_fragSrc)
{
    unsigned int vert = CompileShader(GL_VERTEX_SHADER, p_vertSrc);
    unsigned int frag = CompileShader(GL_FRAGMENT_SHADER, p_fragSrc);

    // BUG CORRIGÉ : si la compilation a échoué, on libère les shaders
    // orphelins et on retourne false plutôt que de tenter un linkage invalide
    if (vert == 0 || frag == 0)
    {
        glDeleteShader(vert);
        glDeleteShader(frag);
        return false;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vert);
    glAttachShader(m_programID, frag);
    glLinkProgram(m_programID);

    // BUG CORRIGÉ : on supprime les shaders APRÈS avoir vérifié le linkage
    // (légal en OpenGL — le program garde sa propre copie après le link)
    glDeleteShader(vert);
    glDeleteShader(frag);

    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(m_programID, 1024, nullptr, log);
        SDL_Log("[Shader] Erreur de linkage : %s", log);
        glDeleteProgram(m_programID);
        m_programID = 0;
    }

    return m_programID != 0;
}

void Shader::Bind()   const { glUseProgram(m_programID); }
void Shader::Unbind() const { glUseProgram(0); }

// p_warn = false pour les uniforms optionnels (ex: uTexture quand pas de texture)
int Shader::GetUniformLocation(const char* p_name, bool p_warn) const
{
    int loc = glGetUniformLocation(m_programID, p_name);
    if (loc == -1 && p_warn)
        SDL_Log("[Shader] Uniform '%s' introuvable", p_name);
    return loc;
}

void Shader::SetInt(const char* p_name, int         p_value) const { glUniform1i(GetUniformLocation(p_name), p_value); }
void Shader::SetFloat(const char* p_name, float       p_value) const { glUniform1f(GetUniformLocation(p_name), p_value); }
void Shader::SetVec2(const char* p_name, float x, float y)    const { glUniform2f(GetUniformLocation(p_name), x, y); }
void Shader::SetVec3(const char* p_name, float x, float y, float z)          const { glUniform3f(GetUniformLocation(p_name), x, y, z); }
void Shader::SetVec4(const char* p_name, float x, float y, float z, float w) const { glUniform4f(GetUniformLocation(p_name), x, y, z, w); }
void Shader::SetMat4(const char* p_name, const float* p_data) const { glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_FALSE, p_data); }

// --- Privées ---

unsigned int Shader::CompileShader(unsigned int p_type, const char* p_source)
{
    unsigned int id = glCreateShader(p_type);
    glShaderSource(id, 1, &p_source, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(id, 1024, nullptr, log);
        const char* typeName = (p_type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        SDL_Log("[Shader] Erreur compilation %s : %s", typeName, log);
        glDeleteShader(id); // BUG CORRIGÉ : libère le shader invalide
        return 0;
    }
    return id;
}

std::string Shader::ReadFile(const std::string& p_path)
{
    std::ifstream file(p_path);
    if (!file.is_open())
    {
        SDL_Log("[Shader] Fichier introuvable : %s", p_path.c_str());
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}