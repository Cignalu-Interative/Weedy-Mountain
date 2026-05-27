#pragma once
#include "2DPendentLib/Transformable.h"
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/Circle.h"
#include "2DPendentLib/Rectangle.h"
#include "2DPendentLib/Color.h"
#include "2DPendentLib/Shape.h"

//enum class Shape
//{
//	Rectangle,
//	Circle
//};

class Collider : public Transformable
{
protected:

	Shape m_shape;

	Color m_color = ColorsNamed::Pink; //Couleur du collider pour le debug

	bool m_isShow = false; //Bool pour savoir si le collider doit être dessiné à l'écran, principalement pour le debug

public :

	Collider() = default;
	virtual ~Collider() = default;

	//virtual bool IsColliding(Collider* p_other) = 0;

	Shape GetShape() const { return m_shape; }
	void SetShape(Shape p_shape) { m_shape = p_shape; }

	void SetColor(Color p_color) { m_color = p_color; }
	Color GetColor() const { return m_color; }

	void SetShow(bool p_show) { m_isShow = p_show; }
	bool IsShow() const { return m_isShow; }
};

//class Collider
//{
//private:
//
//	//Forme du collider
//	Shape m_shape;
//	//Collider de type cercle
//	Circle m_circle;
//	//Collider de type rectangle
//	Rectangle m_rectangle;
//
//public:
//
//	//Constructeur de Collider
//	Collider() = default;
//	//Destructeur de Collider
//	~Collider() = default;
//
//	//Initialiser un collider
//	void Initialize(Shape p_shape, Vector2f p_position, Vector2i p_dimension, Color p_color); 
//
//	//Définir la forme du collider
//	void SetShape(Shape p_type);
//
//	//Obtenir la forme du collider
//	Shape GetShape() const { return m_shape; }
//
//	//Obtenir le collider en fonction de sa forme en const
//	template<typename T>
//	const T& GetShapeCollider() const;
//	//Obtenir le collider en fonction de sa forme
//	template<typename T>
//	T& GetShapeCollider();
//
//	//Définir la position du collider
//	void SetPosition(Vector2f p_position, Vector2f p_anchor = { 0.5,0.5 });
//
//	//Obtenir la position du collider
//	Vector2f GetPosition(Vector2f p_position, Vector2f p_anchor={0.5,0.5}); 
//
//	//Liste des class amie avec la class Collider
//	friend class Entity;
//};
//
//#include "Collider.inl"