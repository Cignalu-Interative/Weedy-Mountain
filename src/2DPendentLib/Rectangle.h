#pragma once
#include <SDL3/SDL.h>
#include "Geometry.h"
#include "Vector2.h"
#include "Drawable.h"

class Rectangle: public Geometry, public Drawable
{
private:

	SDL_FRect m_rect; //Stock les position et dimension du rectangle

	bool m_isFilled = false; //Savoir si le rectangle est rempli ou non

public:

	//Constructeur par défault de rectangle
	Rectangle() = default;
	//Destructeur par default de rectangle
	~Rectangle() = default;

	//Fonction pour initialisée un rectangle
	void Initialize(Vector2f p_position, Vector2i p_dimension, Color p_color);

	void SetFilled(bool p_isFilled) { m_isFilled = p_isFilled; }

	bool IsFilled() const { return m_isFilled; }

	//Fonction pour dessiner un rectangle
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};