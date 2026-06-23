//#pragma once
//#include "glad/glad.h"
//#include <string>
//
//class Texture2D
//{
//public:
//    Texture2D() = default;
//    ~Texture2D();
//
//    bool LoadFromFile(const std::string& p_path);
//
//    void Bind(unsigned int p_slot = 0) const;
//    void Unbind() const;
//
//    int Width()  const { return m_width; }
//    int Height() const { return m_height; }
//    bool IsLoaded() const { return m_id != 0; }
//
//private:
//    unsigned int m_id = 0;
//    int          m_width = 0;
//    int          m_height = 0;
//};