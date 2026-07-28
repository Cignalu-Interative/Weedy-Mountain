#pragma once
#include "NavigationGrid.h"

struct PathNode
{
	Vector2i s_gridPos;

	float s_g;
	float s_h;
	float s_f;
	
	int s_previousPathNode;
};

class Pathfinder
{
private:

	std::vector<PathNode> m_nodePool;
	std::vector<Vector2i> m_neighborPos;
	static constexpr float m_sqrt2 = 1.41421356237;

public:

	std::vector<Tile*> FindPath(NavigationGrid& p_navigationGrid, Vector2i p_start, Vector2i p_goal);

	void GetNeighborPos(Vector2i p_gridDimension, Vector2i p_startPos);

	float StepCost(Vector2i p_previousPos, Vector2i p_currentPos);
};

