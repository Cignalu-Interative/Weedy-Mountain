//#pragma once
//
//#include "glad/glad.h"
//#include <SDL3/SDL.h>
//#include "2DPendentLib/glm/glm.hpp"
//#include "2DPendentLib/glm/gtc/type_ptr.hpp"
//
//// Inclusions nécessaires pour la lecture de fichiers
//#include <fstream>
//#include <sstream>
//#include <string>
//
//// Format des données : x, y, z, r, g, b
//static const float s_triangleVertices[] = {
//     -1.f, -0.75f, 0.0f,  1.0f, 0.0f, 0.0f,  // bas gauche → rouge
//      0.0f, -0.75f, 0.0f,  0.0f, 1.0f, 0.0f,  // bas droite → vert
//      -0.5f,  1.f, 0.0f,  0.0f, 0.0f, 1.0f   // haut centre → bleu
//};
//
//// Nouvelle fonction qui lit un fichier et compile le shader
//static unsigned int CompileShaderFromFile(unsigned int type, const char* filePath)
//{
//    // 1. Récupérer le code source depuis le fichier
//    std::ifstream shaderFile(filePath);
//    if (!shaderFile.is_open())
//    {
//        SDL_Log("Erreur : Impossible d'ouvrir le fichier shader : %s", filePath);
//        return 0;
//    }
//
//    std::stringstream shaderStream;
//    shaderStream << shaderFile.rdbuf();
//    shaderFile.close();
//
//    std::string sourceStr = shaderStream.str();
//    const char* sourceCode = sourceStr.c_str();
//
//    // 2. Compilation classique OpenGL
//    unsigned int id = glCreateShader(type);
//    glShaderSource(id, 1, &sourceCode, nullptr);
//    glCompileShader(id);
//
//    // Vérifier les erreurs de compilation
//    int success;
//    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        char log[512];
//        glGetShaderInfoLog(id, 512, nullptr, log);
//        SDL_Log("Erreur compilation dans %s : %s", filePath, log);
//    }
//    return id;
//}
//
//struct Triangle
//{
//    unsigned int m_vao = 0;
//    unsigned int m_vbo = 0;
//    unsigned int m_program = 0;
//
//    glm::mat4 modelMatrix = glm::mat4(1.0f);
//
//    void Init()
//    {
//        // 1. Créer et remplir le VBO
//        glGenBuffers(1, &m_vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(s_triangleVertices),
//            s_triangleVertices, GL_STATIC_DRAW);
//
//        // 2. Créer le VAO
//        glGenVertexArrays(1, &m_vao);
//        glBindVertexArray(m_vao);
//
//        // Attribut 0 : position
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//
//        // Attribut 1 : couleur
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//
//        // 3. Charger et compiler les fichiers Shaders externes
//        // (Ajuste les chemins selon l'arborescence de ton projet !)
//        unsigned int vert = CompileShaderFromFile(GL_VERTEX_SHADER, "../../src/2DPendentLib/Shaders/TestShader.vert");
//        unsigned int frag = CompileShaderFromFile(GL_FRAGMENT_SHADER, "../../src/2DPendentLib/Shaders/TestShader.frag");
//
//        m_program = glCreateProgram();
//        glAttachShader(m_program, vert);
//        glAttachShader(m_program, frag);
//        glLinkProgram(m_program);
//
//        // Vérifier le linking
//        int success;
//        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
//        if (!success)
//        {
//            char log[512];
//            glGetProgramInfoLog(m_program, 512, nullptr, log);
//            SDL_Log("Erreur link program : %s", log);
//        }
//
//        glDeleteShader(vert);
//        glDeleteShader(frag);
//
//        glBindVertexArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }
//
//    void Draw()
//    {
//        glUseProgram(m_program);
//
//        // Envoi automatique de ta modelMatrix au shader externe avant le dessin
//        int modelLoc = glGetUniformLocation(m_program, "uModel");
//        if (modelLoc != -1)
//        {
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//        }
//
//        glBindVertexArray(m_vao);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//    }
//
//    void Destroy()
//    {
//        glDeleteVertexArrays(1, &m_vao);
//        glDeleteBuffers(1, &m_vbo);
//        glDeleteProgram(m_program);
//    }
//};