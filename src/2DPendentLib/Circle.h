#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Geometry.h"
#include "Vector2.h"
#include "Drawable.h"

class Circle : public Geometry, public Drawable
{
private:

	std::vector<float> m_scanlineCache;  // dx précalculé pour chaque dy

	//Précision du cercle (plus c'est grand plus le cercle est lisse mais plus ça consomme de ressources)
	int m_precision = 32;

	float m_cachedRx = 0.0f;
	float m_cachedRy = 0.0f;

	bool m_isFilled = false; // Si true, le cercle sera dessiné rempli, sinon il sera dessiné en contour

	bool m_clampToScreen = false; // Si true, la geometry sera dessiné à la position la plus proche sur l'écran si sa position dépasse les limites de l'écran

public:

	//Constructeur de cercle par défaut
	Circle() = default;
	//Destructeur par default de circle
	~Circle() = default;

	//Initialiser un cercle
	void Initialize(Vector2f p_position, Vector2i p_dimension, Color p_color);

	void SetPrecision(int p_precision) { m_precision = p_precision; }
	int GetPrecision() const { return m_precision; }

	void SetFilled(bool p_filled) { m_isFilled = p_filled; }

	bool GetFilled() const { return m_isFilled; }

	void SetIsClamp(bool p_isClamp) { m_clampToScreen = p_isClamp; }
	bool GetIsClamp() { return m_clampToScreen; }

	void RebuildCache(float p_rx, float p_ry);

	float SampleCache(float p_localY) const;

	//Draw un cercle
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};