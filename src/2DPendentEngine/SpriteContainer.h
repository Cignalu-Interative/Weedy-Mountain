#pragma once
#include "2DPendentLib/Sprite.h"
#include "2DPendentLib/Vector2.h"
#include <string>

struct SpriteContainer
{
    Sprite* m_sprite = nullptr;
    std::string m_id;
    Vector2f m_offset = { 0.f, 0.f }; 
	int m_zOrder = 0; // Z-order pour le tri des sprites (plus grand = plus proche de la caméra)         
    bool m_visible = true;

    SpriteContainer() = default;
    ~SpriteContainer() { delete m_sprite; m_sprite = nullptr; }

    SpriteContainer(const SpriteContainer&) = delete;
    SpriteContainer& operator=(const SpriteContainer&) = delete;
};