#pragma once
#include "2DPendentEngine/Scene.h"

class CaveScene : public Scene
{
protected:

	void OnInitialize() override;

	void OnEvent() override;

	void OnUpdate() override;

	void OnDestroy() override;

	void OnSleep() override;

	void OnWake() override;

};

