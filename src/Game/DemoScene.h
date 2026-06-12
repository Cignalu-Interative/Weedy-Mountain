#pragma once
#include "2DPendentEngine/Scene.h"
#include <list>
#include <vector>

class User;
class Player;
class DemoScene : public Scene
{
private:

	//Liste de pointeurs vers les joueurs actuellement présents dans la scène
	std::vector<Player*> m_players;
	User* m_user = nullptr;

	Player* m_player = nullptr;
	Player* m_playerToCamera = nullptr;

	Entity* entitySelected = nullptr;
	Entity* lastEntitySelected = nullptr;

	const Vector2i m_levelDimension = { 2000, 2000 }; //Dimension du niveau en pixels

	bool m_test = false;

protected:

	//Fonctions virtuelles héritées de Scene pour définir la logique d'initialisation, de gestion des événements, de mise à jour et de destruction de la scène
	void OnInitialize() override;
	//Fonction pour gérer les événements liés à la scène, notamment en utilisant l'InputManager pour détecter les entrées utilisateur spécifiques à cette scène
	void OnEvent() override;
	//Fonction pour mettre à jour la logique de la scène, notamment en mettant à jour les joueurs et les autres éléments présents dans la scène
	void OnUpdate() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de mise en veille
	void OnSleep() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de réveil
	void OnWake() override;
	//Fonction pour libérer les ressources utilisées par la scène, notamment en détruisant les joueurs et les autres éléments présents dans la scène
	void OnDestroy() override;

public:

	DemoScene();
	~DemoScene() = default;

	void TestVélocité();

	void SelectEntity(Vector2f p_position);
};