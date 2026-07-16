#pragma once
#include "NavigationGrid.h"

struct PathNode
{
	Vector2i s_gridPos;

	float s_g;
	float s_h;
	float s_f;
	
	PathNode* s_previusPathNode;
};

class Pathfinder
{
public:

	std::vector<Tile*> FindPath(NavigationGrid& p_navigationGrid, Vector2i p_start, Vector2i p_goal);

	std::vector<Vector2i> GetNeighborPos(Vector2i p_gridDimension, Vector2i p_startPos);
};

