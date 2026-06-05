#pragma once
#include "2DPendentEngine/Scene.h"
#include "Player.h"
class GameScene : public Scene
{
private:
	Player* m_Player = nullptr;

protected:

	void OnInitialize() override;
	
	void OnEvent() override;

	void OnUpdate() override;
	
	void OnDestroy() override;

	virtual void OnSleep()override {};
	//Fonction virtuelle pure pour permettre aux classes qui héritent de Scene de définir leur propre logique de réveil
	virtual void OnWake() override {};
public:


};

