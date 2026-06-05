#pragma once
#include "Scene.h"

class TestScene : public Scene
{
protected:

	//Fonctions virtuelles h�rit�es de Scene pour d�finir la logique d'initialisation, de gestion des �v�nements, de mise � jour et de destruction de la sc�ne
	void OnInitialize() override;
	//Fonction pour g�rer les �v�nements li�s � la sc�ne, notamment en utilisant l'InputManager pour d�tecter les entr�es utilisateur sp�cifiques � cette sc�ne
	void OnEvent() override;
	//Fonction pour mettre � jour la logique de la sc�ne, notamment en mettant � jour les joueurs et les autres �l�ments pr�sents dans la sc�ne
	void OnUpdate() override;
	//Fonction pour lib�rer les ressources utilis�es par la sc�ne, notamment en d�truisant les joueurs et les autres �l�ments pr�sents dans la sc�ne
	void OnDestroy() override;

public:

	TestScene() = default;
	~TestScene() = default;
};

