#pragma once
#include "2DPendentEngine/Entity.h"

class Player : public Entity
{
private:



protected:

	//Fonction pour initialiser le joueur, notamment en définissant sa position, sa dimension, son sprite et son collider
	void OnInitialize() override;
	//Fonction pour gérer les événements liés au joueur, notamment en gérant les entrées du joueur pour le déplacer ou effectuer des actions
	void OnEvent() override;
	//Fonction pour mettre à jour le joueur, notamment en mettant à jour son sprite et en gérant sa logique de jeu
	void OnUpdate() override;
	//Fonction pour gérer les collisions du joueur avec d'autres entités, notamment en gérant les interactions avec les ennemis, les objets ou les éléments du décor
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide _side) override;
	//Fonction pour marquer le joueur pour destruction, elle sera détruite à la fin de la frame en cours
	void OnDestroy() override;

public:



};

