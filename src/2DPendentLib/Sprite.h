#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <chrono>
#include "Vector2.h"
#include "Transformable.h"
#include "Drawable.h"

class Sprite : public Transformable, public Drawable
{
private:

	SDL_Texture* m_texture = nullptr; //Texture du sprite
	SDL_FRect m_spriteRect; //Rectangle de la texture à dessiner

	bool m_isGUI = false; //Bool pour savoir si le sprite est un élément d'interface utilisateur (GUI), il ne sera pas affecté par la physique et les collisions
	//SDL_FPoint m_rotationCenter = { 0, 0 }; //Point autour duquel le sprite tourne

	//Vector2i m_frameSize; // Taille d'une frame
	//Vector2f m_framesNB; // NB de frames en X et Y
	//int m_totalFrames; // NB total de frames dans le sprite sheet
	//Vector2f m_currentFrame; // Index de la frame courante (en partant de 0,0 en haut à gauche)

	//bool m_facingRight = true; // Bool pour savoir si le sprite fait face à droite ou à gauche (pour le flip horizontal)
	//float m_animTimer = 0.0f;   // Timer animation
	//float m_speedAnimation;     // Vitesse de l'animation (ex: 0.1f pour 10 FPS)
	//int m_idleStartFrame = 0;   // Début idle (ex: 0)
	//int m_idleEndFrame = 3;     // Fin idle (ex: 3)
	//int m_walkStartFrame = 4;   // Début walk (ex: 4)
	//int m_walkEndFrame = 11;    // Fin walk (ex: 11)

	//Vector2f m_anchor = { 0.5f, 0.5f }; // Ancre du sprite

public:

	Sprite() = default; //Constructeur par défaut de Sprite
	~Sprite(); //Destructeur de Sprite

	//Initialise le sprite en chargeant la texture à partir du chemin donné, en définissant sa position, sa dimension et en configurant les paramètres d'animation
	void Initialise(const std::string& p_id, Vector2f p_position, Vector2i p_dimension, Render* f_render);

	void SetIsGUI(bool p_isGUI) { m_isGUI = p_isGUI; }
	bool IsGUI() const { return m_isGUI; }
	
	/*
	//Met à jour l'animation du sprite en fonction du temps écoulé depuis la dernière frame
	void Update(float p_deltaTimeMs);

	//Permet de régler automatiquement la taille d'une frame en fonction de la dimension du sprite et du nombre de frames en X et Y
	void AutoDetectFrameSize(Vector2i p_spriteDimention);

	//Positionne le sprite sur la frame du sprite sheet correspondant à l'index donné (en partant de 0,0 en haut à gauche)
	void SetCurrentFramePosition(Vector2f p_framePosition);
	//Obtenir la position de la frame courante dans le sprite sheet
	Vector2f GetCurrentFramePosition() const { return m_currentFrame; }
	//Obtenir l'index de la frame courante dans le sprite sheet
	int GetCurrentFrameIndex() const;

	//Permet de faire avancer l'animation du sprite en passant à la frame suivante, en bouclant entre les frames de début et de fin spécifiées
	void NextFrame(int p_startFrame = 0, int p_endFrame = -1);

	//Set le bool pour savoir si le sprite fait face à droite ou à gauche (pour le flip horizontal)
	void SetFacingRight(bool p_facingRight);
	bool IsFacingRight() const; //Obtenir le bool pour savoir si le sprite fait face à droite ou à gauche (pour le flip horizontal)

	// Permet de régler la vitesse de l'animation (ex: 0.1f pour 10 FPS)
	void SetSpeedAnimation(float p_speed);
	// Obtenir la vitesse de l'animation
	float GetSpeedAnimation() const { return m_speedAnimation; }*/

	//Fonction pour dessinner le sprite à l'écran
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

	/*
	//Permet de définir l'ancre du sprite (ex: (0,0) pour le coin haut gauche, (0.5,0.5) pour le centre, etc.)
	void SetAnchor(Vector2f p_anchor) { m_anchor = p_anchor; }
	//Obtenir l'ancre du sprite
	Vector2f GetAnchor() const { return m_anchor; }*/
};