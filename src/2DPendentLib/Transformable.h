#pragma once
#include "Vector2.h"
//Version 2.0.0

class Transformable
{
protected:

	Vector2f m_position; //sert a stocker la position pour la class qui hérite de transformable
	//Point autour duquel l'objet tourne (ex: (0,0) pour le coin haut gauche, (0.5,0.5) pour le centre, etc.)
	Vector2f m_rotationPoint = { 0.5f, 0.5f };

	Vector2i m_dimension; //sert a stocker la dimention pour la class qui hérite de transformable

	float m_distance = 0.0f; //Principalement pour la spatialisation du son, stocker la distance entre le son et le joueur, en pourcentage (0 = proche, 255 = loin)
	float m_angle = 0.0f; // stocker l'angle de rotation
	//Pour connaitre vers ou l'élément se déplace
	//La vélocité est un vecteur qui indique la direction et la vitesse de déplacement d'un objet.
	//Avec comme unités de mesure des unités de distance par unité de temps (par exemple, pixels par seconde).
	//Vector2f m_velocity = { 0.0f, 0.0f };

public:

	//Constructeur par défault de transformable
	Transformable() = default; 

	//Constructeur de transformable
	Transformable(Vector2f p_position, Vector2i p_dimension); 
	//Destructeur de transformable
	~Transformable() = default; 

	//Permet de changer l'angle de rotation
	void SetAngle(float p_angle);
	//Angle actuel de l'objet
	float GetAngle() const { return m_angle; }
	//Point autour duquel l'objet tourne
	Vector2f GetRotationPoint() const { return m_rotationPoint; }

	//Definir une dimention en Vector2
	void SetDimension(Vector2i p_dimension); 
	//Resize une nouvelle dimention en Vector2
	void Resize(Vector2i p_newDimension); 
	//Obtenir la dimention en Vector2
	Vector2i GetDimension() const { return m_dimension; }

	//Définir une position et son ancre (Par défaut = 0.5f, 0.5f) en Vector2f  
	void SetPosition(const Vector2f& p_position, Vector2f p_anchor = { 0.5f, 0.5f });
	//Obtenir la position en fonction de l'ancre (Par défaut = 0.5f, 0.5f) en Vector2f
	Vector2f GetPosition(Vector2f p_anchor = { 0.5f, 0.5f }) const; 

	//Définir la distance principalement pour la spatialisation du son
	void SetDistance(float p_distance) { m_distance = p_distance; }
	//Obtenir la distance principalement pour la spatialisation du son
	float GetDistance() const { return m_distance; }

	void Move(Vector2f p_translation);
};

