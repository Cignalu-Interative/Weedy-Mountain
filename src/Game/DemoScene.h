#pragma once
#include "2DPendentEngine/Scene.h"
#include <list>
#include <vector>

class User;
class Player;
class Inventory;
class Hotbar;
class PlayerInventory;
class DemoScene : public Scene
{
private:

	//Liste de pointeurs vers les joueurs actuellement pr�sents dans la sc�ne
	std::vector<Player*> m_players;
	User* m_user = nullptr;

	Player* m_playerToCamera = nullptr;
	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;

	Entity* entitySelected = nullptr;
	Entity* lastEntitySelected = nullptr;

	const Vector2i m_levelDimension = { 2000, 2000 }; //Dimension du niveau en pixels





	Inventory* m_inv = nullptr;
	Hotbar* m_hotbar = nullptr;

	PlayerInventory* m_playerInventory = nullptr;

protected:

	//Fonctions virtuelles h�rit�es de Scene pour d�finir la logique d'initialisation, de gestion des �v�nements, de mise � jour et de destruction de la sc�ne
	void OnInitialize() override;
	//Fonction pour g�rer les �v�nements li�s � la sc�ne, notamment en utilisant l'InputManager pour d�tecter les entr�es utilisateur sp�cifiques � cette sc�ne
	void OnEvent() override;
	//Fonction pour mettre � jour la logique de la sc�ne, notamment en mettant � jour les joueurs et les autres �l�ments pr�sents dans la sc�ne
	void OnUpdate() override;
	//Fonction virtuelle pure pour permettre aux classes qui h�ritent de Scene de d�finir leur propre logique de mise en veille
	void OnSleep() override;
	//Fonction virtuelle pure pour permettre aux classes qui h�ritent de Scene de d�finir leur propre logique de r�veil
	void OnWake() override;
	//Fonction pour lib�rer les ressources utilis�es par la sc�ne, notamment en d�truisant les joueurs et les autres �l�ments pr�sents dans la sc�ne
	void OnDestroy() override;

public:

	DemoScene();
	~DemoScene() = default;

	void TestV�locit�();

	void SelectEntity(Vector2f p_position);
};