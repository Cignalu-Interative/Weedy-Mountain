#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Geometry.h"
#include "Vector2.h"
#include "Drawable.h"

class Rectangle: public Geometry, public Drawable
{
private:

	SDL_FRect m_rect; //Stock les position et dimension du rectangle

	bool m_isFilled = false; //Savoir si le rectangle est rempli ou non

	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

public:

	//Constructeur par défault de rectangle
	Rectangle() = default;
	//Destructeur par default de rectangle
	~Rectangle() = default;

	//Fonction pour initialisée un rectangle
	void Initialize(Vector2f p_position, Vector2i p_dimension, Color p_color);

	void SetFilled(bool p_isFilled) { m_isFilled = p_isFilled; }

	bool IsFilled() const { return m_isFilled; }

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	//Fonction pour dessiner un rectangle
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};