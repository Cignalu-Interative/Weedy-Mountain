#include "pch.h"
//#include "SpriteOpenGL.h"
//#include "Utils.h"
//
//static const char* s_spriteVert = R"glsl(
//#version 450 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec2 aTexCoord;
//out vec2 vTexCoord;
//uniform mat4 uModel;
//uniform mat4 uProjection;
//void main() {
//    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
//    vTexCoord = aTexCoord;
//}
//)glsl";
//
//static const char* s_spriteFrag = R"glsl(
//#version 450 core
//in vec2 vTexCoord;
//out vec4 FragColor;
//uniform sampler2D uTexture;
//uniform vec4 uColor;
//void main() {
//    FragColor = texture(uTexture, vTexCoord) * uColor;
//}
//)glsl";
//
//void SpriteOpenGL::Init(int p_screenWidth, int p_screenHeight)
//{
//    // Quad centré sur l'origine (le modèle sera mis à l'échelle)
//    std::vector<float> verts = {
//        // x     y     z     u     v
//        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
//         0.5f, -0.5f, 0.0f,  1.0f, 1.0f,
//         0.5f,  0.5f, 0.0f,  1.0f, 0.0f,
//        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
//    };
//    std::vector<unsigned int> idx = { 0, 1, 2,  2, 3, 0 };
//    std::vector<VertexAttrib> attribs = { {0, 3}, {1, 2} };
//
//    m_quadMesh.Upload(verts, attribs, idx);
//    m_shader.LoadFromSource(s_spriteVert, s_spriteFrag);
//
//    // Projection ortho — origine en haut à gauche
//    Utils::MakeOrtho(m_projection, 0.f, (float)p_screenWidth,
//        (float)p_screenHeight, 0.f, -1.f, 1.f);
//
//    // Activer le blending pour la transparence PNG
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void SpriteOpenGL::DrawSprite(const Texture2D& p_texture,
//    float px, float py,
//    float width, float height,
//    float angle,
//    float r, float g, float b, float a)
//{
//    float model[16];
//    Utils::MakeModel2D(model, px + width * 0.5f, py + height * 0.5f,
//        width, height, angle);
//
//    m_shader.Bind();
//    m_shader.SetMat4("uProjection", m_projection);
//    m_shader.SetMat4("uModel", model);
//    m_shader.SetVec4("uColor", r, g, b, a);
//    m_shader.SetInt("uTexture", 0);
//
//    p_texture.Bind(0);
//    m_quadMesh.Draw();
//}
//
//void SpriteOpenGL::Destroy()
//{
//    m_quadMesh.Destroy();
//    m_shader.~Shader(); // ou une méthode Destroy() si tu en ajoutes une
//}