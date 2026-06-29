#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
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
	SDL_FRect* m_srcRect = nullptr;

	bool m_isGUI = false; //Bool pour savoir si le sprite est un élément d'interface utilisateur (GUI), il ne sera pas affecté par la physique et les collisions

	bool m_isVisible = true; //Bool pour savoir si le sprite doit être dessiné à l'écran

	Uint8 m_oppacity = 255; //Opacité du sprite, de 0 (transparent) à 255 (opaque)

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

	void SetIsVisible(bool p_isVisible) { m_isVisible = p_isVisible; }
	bool IsVisible() const { return m_isVisible; }

	void ChangeTexture(const std::string& p_texturePath);

	void SetOpacity(Uint8 p_oppacity);
	Uint8 GetOpacity() const { return m_oppacity; }

	SDL_FRect* GetSpriteRect() { return m_srcRect; }
	void SetSourceRectSprite(SDL_FRect p_SDLrect);

	//Fonction pour dessinner le sprite à l'écran
	void Draw(Render* f_render, Window* f_window, Camera* p_camera) override;

};