#include "pch.h"
//#include "Texture2D.h"
//#include <SDL3/SDL_main.h>
//#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
//
//Texture2D::~Texture2D()
//{
//    if (m_id != 0)
//        glDeleteTextures(1, &m_id);
//}
//
//bool Texture2D::LoadFromFile(const std::string& p_path)
//{
//    // Charger l'image avec SDL_image
//    SDL_Surface* surface = IMG_Load(p_path.c_str());
//    if (!surface)
//    {
//        SDL_Log("[Texture2D] Erreur IMG_Load : %s", SDL_GetError());
//        return false;
//    }
//
//    // Convertir en RGBA32 pour simplifier (OpenGL aime ce format)
//    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
//    SDL_DestroySurface(surface);
//    if (!converted) return false;
//
//    m_width = converted->w;
//    m_height = converted->h;
//
//    // Créer la texture OpenGL
//    glGenTextures(1, &m_id);
//    glBindTexture(GL_TEXTURE_2D, m_id);
//
//    // Paramètres de filtrage
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // pixel art → NEAREST
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    // Uploader les pixels vers le GPU
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
//        m_width, m_height, 0,
//        GL_RGBA, GL_UNSIGNED_BYTE,
//        converted->pixels);
//
//    glGenerateMipmap(GL_TEXTURE_2D);  // optionnel pour le pixel art
//
//    glBindTexture(GL_TEXTURE_2D, 0);
//    SDL_DestroySurface(converted);
//
//    SDL_Log("[Texture2D] Chargée : %s (%dx%d)", p_path.c_str(), m_width, m_height);
//    return true;
//}
//
//void Texture2D::Bind(unsigned int p_slot) const
//{
//    glActiveTexture(GL_TEXTURE0 + p_slot);
//    glBindTexture(GL_TEXTURE_2D, m_id);
//}
//
//void Texture2D::Unbind() const
//{
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
