#pragma once
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Transformable.h"
#include "2DPendentLib/Drawable.h"
#include "Color.h"

class Entity;
class Camera : public Transformable, public Drawable
{
private:

	float m_maxZoom = 5.0f; // Zoom maximum autorisé
	float m_minZoom = 0.2f; // Zoom minimum autorisé
	float m_zoomFactor = 1.0f; //Facteur de zoom de la caméra, par défaut à 1.0f (pas de zoom)

	Color m_color = ColorsNamed::DarkTurquoise; // Couleur de la caméra pour le debug

	bool m_isShow = false;

	float m_smoothSpeed = 1;

	Entity* m_followedEntity = nullptr;

public:

	//Constructeur de Camera, il prend en paramètre la position et la dimension de la caméra
	Camera(Vector2i p_dimension, Vector2f p_position = Vector2f(0.0f, 0.0f), Vector2f p_anchor = Vector2f(0.0f, 0.0f), float p_zoomFactor = 1.0f);
	//Destructeur de Camera
	~Camera() = default;

	// Set le zoom de la caméra, plus le facteur est grand, plus la caméra est zoomée (ex: 2.0f = zoom x2, 0.5f = zoom x0.5)
	void SetZoom(float p_zoomFactor);
	float GetZoom() const { return m_zoomFactor; }
	void ZoomIn(float p_amount);
	void ZoomOut(float p_amount);

	void SetFollowedEntity(Entity* p_entity) { m_followedEntity = p_entity; }

	void Follow(float p_dt);

	//MONDE -> ECRAN
	template<typename T>
	Vector2<T> WorldToScreen(Vector2<T> p_worldPos) const;

	//ECRAN -> MONDE
	template<typename T>
	Vector2<T> ScreenToWorld(Vector2<T> p_screenPos) const;

	void SetIsShow(bool p_isShow) { m_isShow = p_isShow; }
	bool GetIsShow() const { return m_isShow; }

	void SetSmoothSpeed(float p_smoothSpeed) { m_smoothSpeed = p_smoothSpeed; }
	float GetSmoothSpeed() const { return m_smoothSpeed; }

	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;
};

#include "Camera.inl"