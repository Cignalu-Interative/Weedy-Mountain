#pragma once
#include "glad/glad.h"
#include <string>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    // Charge depuis des fichiers .vert / .frag
    bool LoadFromFiles(const std::string& p_vertPath,
        const std::string& p_fragPath);

    // Charge depuis des strings en mémoire
    bool LoadFromSource(const char* p_vertSrc, const char* p_fragSrc);

    void Bind()   const;
    void Unbind() const;

    // --- Setters d'uniforms ---
    void SetInt(const char* p_name, int p_value)          const;
    void SetFloat(const char* p_name, float p_value)        const;
    void SetVec2(const char* p_name, float x, float y)     const;
    void SetVec3(const char* p_name, float x, float y, float z) const;
    void SetVec4(const char* p_name, float x, float y, float z, float w) const;
    void SetMat4(const char* p_name, const float* p_data)  const;

    bool IsValid() const { return m_programID != 0; }

private:
    unsigned int m_programID = 0;

    static unsigned int CompileShader(unsigned int p_type, const char* p_source);
    static std::string  ReadFile(const std::string& p_path);
    int GetUniformLocation(const char* p_name, bool p_warn = true) const;
};
