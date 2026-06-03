#include "pch.h"
#include "EnviroProps.h"
#include "Collision.h"
#include "Player.h"

void EnviroProps::OnInitialize()
{
	
}

void EnviroProps::OnEvent()
{
}

void EnviroProps::OnUpdate()
{
	Button::OnUpdate();
}

void EnviroProps::OnCollision(Entity* other)
{
	Collider* otherCollider;

	if (auto cast = dynamic_cast<Player*>(other))
	{
		otherCollider = cast->GetInteractZone();
	}
	else
		return;


	Collision::IsColliding(*m_collider, *otherCollider);
}

void EnviroProps::OnDestroy()
{
}

void EnviroProps::SetType(EnviroName p_name)
{
	m_Name = p_name;

	switch (m_Name) {
	case EnviroName::Tree:
		m_Stats.m_MinSpawn = 1;
		m_Stats.m_MaxSpawn = 3;
		m_Stats.m_HitToDestroy = 4;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Shrub:
		m_Stats.m_MinSpawn = 2;
		m_Stats.m_MaxSpawn = 4;
		m_Stats.m_HitToDestroy = 2;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Grass:
		m_Stats.m_MinSpawn = 3;
		m_Stats.m_MaxSpawn = 5;
		m_Stats.m_HitToDestroy = 1;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Rock:
		m_Stats.m_MinSpawn = 0;
		m_Stats.m_MaxSpawn = 0;
		m_Stats.m_HitToDestroy = 2;
		//m_Stats.m_MinToolsRarity
		break;
	case EnviroName::Stone:
		m_Stats.m_MinSpawn = 0;
		m_Stats.m_MaxSpawn = 0;
		m_Stats.m_HitToDestroy = 4;
		//m_Stats.m_MinToolsRarity
		break;
	}

	SetOnClick([this] {
		m_Stats.m_HitToDestroy--;
		if (m_Stats.m_HitToDestroy <= 0) {
			
			Destroy();
		}
		});
}
