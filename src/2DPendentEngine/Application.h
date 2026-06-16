#pragma once
#include <chrono>
#include <thread>
#include <vector>
#include <list>
#include "2DPendentLib/Window.h"
#include "2DPendentLib/Render.h"
#include "2DPendentLib/Debug.h"
#include "2DPendentLib/Vector2.h"
#include "Timer.h"
#include "2DPendentLib/Sprite.h"
#include "SceneManager.h"
#include "Collision.h"
#include "SoundManager.h"
#include "Aleatory.h"
#include "2DPendentLib/AssetManager.h"


#include "2DPendentLib/glad/glad.h"
#include "2DPendentLib/glm/glm.hpp"
#include "2DPendentLib/glm/gtc/type_ptr.hpp"
#include "2DPendentLib/Triangle.h"
#include "2DPendentLib/Shader.h"
#include "2DPendentLib/Mesh.h"
#include "2DPendentLib/Texture2D.h"
#include "2DPendentLib/SpriteOpenGL.h"

#include "2DPendentLib/TVVertex.h"

//Version 3.4.0

class Entity;
class Scene;
class Camera;
class Application
{
private:
	// Window membre de la classe Application, utilisé pour créer une fenêtre de jeu et gérer les événements liés à la fenêtre
	Window m_window;
	// Render membre de la classe Application, utilisé pour dessiner les éléments graphiques à l'écran
	Render m_render;

	// vector de pointeurs vers les entités actuellement présentes dans le jeu
	std::vector<Entity*> m_entities;
	// vector de pointeurs vers les entités à détruire
	std::vector<Entity*> m_entityToDestroy;
	// vector de pointeurs vers les entités à ajouter
	std::vector<Entity*> m_entityToAdd;

	//Timer pour gérer les FPS
	Timer m_timerFPS;
	Timer m_timerUpdate; // Timer pour gérer le temps écoulé depuis la dernière mise à jour du jeu

	Camera* m_camera; //Camera du jeu, utilisée pour gérer la vue du jeu et les déplacements de la caméra

	//m_deltaTimeMs membre de la classe Application, utilisé pour stocker le temps écoulé entre deux frames en milliseconds
	float m_deltaTime = 0.0f;
	float m_targetDeltaTime = 1.0f / m_targetFPS; // 1 seconde divisée par le nombre de frames par seconde pour obtenir le temps cible entre chaque frame

	int m_targetFPS = 64; // 64 par défaut Pour éviter la division par 0 avant d'etre calculer pas la fonction V_Sync
	// FPS counter
	int m_frameCount = 0;

	// FPS actuel calculé en utilisant un lissage exponentiel pour éviter les fluctuations brusques
	float m_currentFPS = 0.0f;
	//Défini le temps entre chaque mise à jour du compteur de FPS, en secondes (par exemple, 1.0f pour mettre à jour le compteur de FPS toutes les secondes)
	float m_timeToUpdateFPS = 1.0f;
	//Timer pour gérer le temps écoulé depuis la dernière mise à jour du compteur de FPS
	float m_timeLastUpdateFPS = 0.0f;
	float m_accumulator = 0.0f; // Accumulateur de temps pour gérer les mises à jour fixes du jeu

	const float m_FIX_UPDATE = 1.0f / 60.0f; // Temps fixe entre chaque mise à jour du jeu, en secondes (par exemple, 1/60 pour une mise à jour à 60 FPS)

	//m_isRunning membre de la classe Application, utilisé pour savoir si le jeu est en cours d'exécution ou s'il doit se fermer
	bool m_isRunning = false;
	bool m_autoFPS = true; // Si true, le jeu ajustera automatiquement le nombre de frames par seconde par rapport au fps trouvé grace à la fonction V_Sync
	bool m_drawFPS = true; //Bool pour savoir si le compteur de FPS doit être dessiné à l'écran, utile pour le debug


	//Triangle triangle;
	//Shader shader;
	//Mesh quadMesh;
	//Texture2D texture;
	//SpriteOpenGL spriteOpenGL;

	//// Position (x,y,z) + Couleur (r,g,b)
	//std::vector<float> quadVerts = {
	//	// pos              couleur
	//	0.0f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f,  // rouge en bas a gauche
	//	 0.98f, -0.75f, 0.0f,   0.0f, 1.0f, 0.0f,  // verten bas a droite
	//	 0.98f,  0.75f, 0.0f,   0.0f, 0.0f, 1.0f,  // bleu en haut a droite
	//	0.0f,  0.75f, 0.0f,   1.0f, 1.0f, 0.0f   // jaune en haut a gauche
	//};

	//std::vector<unsigned int> quadIndices = {
	//	0, 1, 2,
	//	2, 3, 0
	//};

	//// Attributs : position (loc 0) + couleur (loc 1)
	//std::vector<VertexAttrib> attribs = {
	//	{ 0, 3 },  // location 0 → position (vec3)
	//	{ 1, 3 },  // location 1 → couleur (vec3)
	//};

	std::vector<TVVertex> m_tvVertexVector;

	TVVertex m_testTVVertex;

	Timer m_timeOpenGL;

public:

	//Constructeur de l'Application
	Application();
	//Destructeur de l'Application, il est responsable de la libération de toutes les ressources utilisées par l'application pour éviter les fuites de mémoire
	~Application();

	//Singleton de l'Application
	static Application* Get();

	//Fonction pour initialiser l'application, notamment en créant la fenêtre de jeu, le render et en configurant les différentes parties du moteur
	void Init();
	//Fonction pour lancer la boucle de jeu principale
	void Run();
	//Fonction pour arrêter l'application et libérer les ressources
	void Shutdown();

	// si m_autoFPS est true, le jeu ajustera automatiquement le nombre de frames par seconde par rapport au fps trouvé grace à la fonction V_Sync
	void FPSDeltaTime();
	//Fonction pour gérer les entrées utilisateur, notamment en utilisant l'InputManager pour détecter les événements liés au clavier, à la souris et aux manettes de jeu
	void HandleInput();
	//Fonction pour mettre à jour les différentes parties du jeu, notamment en mettant à jour les entités, les scènes et les éléments de debug
	void Update();
	//Fonction pour dessiner les différentes parties du jeu, notamment en dessinant les entités, les scènes et les éléments de debug
	void Draw();

	//Fonction pour définir le nombre de frames par seconde cible
	void SetFPS(int p_targetFPS); 
	//Fonction pour limiter les fps en fonction du nombre de frames par seconde cible et du temps écoulé entre deux frames
	void CapFPS(); 
	//Fonction pour mettre à jour le compteur de FPS en utilisant un lissage exponentiel pour éviter les fluctuations brusques
	// (doit être appelé à chaque frame avec le temps écoulé depuis la dernière frame en nanosecondes)
	void UpdateFPSCounter();

	int GetCurrentFPS() const { return static_cast<int>(m_currentFPS); }

	void IsEneableToUpdate();

	void UpdatePhysics_Collisions();

	//Obtention du render
	Render* GetRender() { return &m_render; }

	Window* GetWindow() { return &m_window; }

	Camera* GetCamera() { return m_camera; }

	void SetisRunning(bool p_isRunning) { m_isRunning = p_isRunning; }
	bool GetisRunning() const { return m_isRunning; }

	void SetDrawFPS(bool p_drawFPS) { m_drawFPS = p_drawFPS; }
	bool GetDrawFPS() const { return m_drawFPS; }


public:

	//Liste des classes amies avec la classe Application, ces classes peuvent accéder aux membres privés de l'Application
	friend class Entity;
	friend class Debug;
	friend class Scene;
	friend class SceneManager;
	friend class AssetManager;
};