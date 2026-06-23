#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include "Vector2.h"
#include "Color.h"
#include "Window.h"
#include "Transformable.h"

class Geometry : public Transformable
{
protected:

	std::string m_id; //Identifiant de la geometry

	//Couleur de la geometry
	Color m_color;

	bool m_isGUI = false; // Si true, la geometry sera dessiné en GUI, sinon elle sera dessiné en jeu

	bool m_isVisible = true; // Si false, la geometry ne sera pas dessiné	

public:

	//Constructeur de géometry par defaut
	Geometry() = default;

	//Definis la position, la dimention et la couleur d'une geometry
	Geometry(Vector2f p_position, Vector2i p_dimension, Color p_color);

	//Destructeur de geometry
	virtual ~Geometry() = default;

	//Définis la couleur de la geometry
	void SetColor(Color p_color) { m_color = p_color; }
	Color GetColor() const { return m_color; }

	void SetId(std::string p_id) { m_id = p_id; }
	std::string GetId() const { return m_id; }

	void SetIsGUI(bool p_isGUI) { m_isGUI = p_isGUI; }
	bool GetIsGUI() const { return m_isGUI; }
	
	void SetIsVisible(bool p_isVisible) { m_isVisible = p_isVisible; }
	bool GetIsVisible() const { return m_isVisible; }

};