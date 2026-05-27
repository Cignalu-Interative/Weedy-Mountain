
#include "SoundManager.h"
//#include "2DPendentLib/Sound.h"

template <typename T>
float SoundManager::GetDistanceTo(Sound* p_sound, T* p_entity)
{
	static_assert(std::is_base_of_v<Entity, T>, "T doit être une class Entity ou une classe dérivée de Entity !");

	float dx = p_sound->GetPosition().GetX() - p_entity->GetPosition().GetX();
	float dy = p_sound->GetPosition().GetY() - p_entity->GetPosition().GetY();

	return sqrt(dx * dx + dy * dy);
}

template<typename T>
inline void SoundManager::SetListener(T* p_entity)
{
	static_assert(std::is_base_of_v<Entity, T>, "T doit être une class Entity ou une classe dérivée de Entity !");

	m_listener = p_entity;
}