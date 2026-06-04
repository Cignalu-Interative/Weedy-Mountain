#pragma once
#include "2DPendentEngine/Scene.h"

class Player;
class TestScene : public Scene
{
private:


protected:

	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique d'initialisation
	void OnInitialize() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de gestion des événements
	void OnEvent() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de mise à jour
	void OnUpdate() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de mise en veille
	void OnSleep() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de réveil
	void OnWake() override;
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de destruction
	void OnDestroy() override;


public:



};

