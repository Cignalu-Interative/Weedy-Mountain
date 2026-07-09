#pragma once
#include <SDL3/SDL.h>
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

	int m_drawLayer = 1;

	int m_type = -1; // Type de geometry 0 = Point, 1 = Line, 2 = Rectangle, 3 = FillRectangle, 4 = Circle, 5 = FillCircle, 6 = Text

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

	void SetDrawLayer(int p_drawLayer) { m_drawLayer = p_drawLayer; }
	int GetDrawLayer() const { return m_drawLayer; }

	void SetType(int p_type) { m_type = p_type; }
	int GetType() const { return m_type; }

};