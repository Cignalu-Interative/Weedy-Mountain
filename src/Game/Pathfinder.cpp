#include "pch.h"
#include "Pathfinder.h"
#include <algorithm>
#include <queue>

std::vector<Tile*> Pathfinder::FindPath(NavigationGrid& p_navigationGrid, Vector2i p_start, Vector2i p_goal)
{
	std::cout << "Euuuuhhhh je calcule le chemin !!" << std::endl;

	m_nodePool.clear();
	m_nodePool.reserve(p_navigationGrid.GetGridDimension().GetX() * p_navigationGrid.GetGridDimension().GetY());

	m_neighborPos.clear();

	if (p_navigationGrid.GetTileByGridPos(p_goal)->canBeCrossed == false)
	{
		return {};
	}

	std::vector<Tile*> path;

	auto comparator = [this](int a, int b) { return m_nodePool[a].s_f > m_nodePool[b].s_f; };
	std::priority_queue<int, std::vector<int>, decltype(comparator)> toExplore(comparator);

	float g = 0.f;

	PathNode previousNode;

	previousNode.s_gridPos = p_start;

	previousNode.s_g = g;

	float dx = std::abs(p_start.GetX() - p_goal.GetX());
	float dy = std::abs(p_start.GetY() - p_goal.GetY());

	previousNode.s_h = Utils::Max({ dx, dy }) + (m_sqrt2 - 1) * Utils::Min({ dx, dy });

	previousNode.s_f = previousNode.s_g + previousNode.s_h;

	previousNode.s_previousPathNode = -1;
	
	m_nodePool.push_back(previousNode);
	int startIndex = (int)(m_nodePool.size() - 1);

	int currentIndex = 0;

	Vector2i dim = p_navigationGrid.GetGridDimension();
	std::vector<bool> alreadyExplore(dim.GetX() * dim.GetY(), false);

	toExplore.push(currentIndex);

	while (toExplore.size() > 0)
	{
		currentIndex = toExplore.top();
		toExplore.pop();

		if (alreadyExplore[m_nodePool[currentIndex].s_gridPos.GetX() * dim.GetY() + m_nodePool[currentIndex].s_gridPos.GetY()])
		{
			continue;
		}

		alreadyExplore[m_nodePool[currentIndex].s_gridPos.GetX() * dim.GetY() + m_nodePool[currentIndex].s_gridPos.GetY()] = true;

		if (m_nodePool[currentIndex].s_gridPos == p_goal)
		{
			int tempIndex = currentIndex;

			while (tempIndex != -1)
			{
				path.push_back(p_navigationGrid.GetTileByGridPos(m_nodePool[tempIndex].s_gridPos));
				tempIndex = m_nodePool[tempIndex].s_previousPathNode;
			}

			std::reverse(path.begin(), path.end());

			return path;
		}

		GetNeighborPos(p_navigationGrid.GetGridDimension(), m_nodePool[currentIndex].s_gridPos);

		for (Vector2i pos : m_neighborPos)
		{
			Tile* tile = p_navigationGrid.GetTileByGridPos(pos);

			if (tile == nullptr)
			{
				continue;
			}

			if (tile->canBeCrossed &&
				alreadyExplore[pos.GetX() * dim.GetY() + pos.GetY()] == false)
			{
				g = StepCost(m_nodePool[currentIndex].s_gridPos, pos);

				float dx = std::abs(pos.GetX() - p_goal.GetX());
				float dy = std::abs(pos.GetY() - p_goal.GetY());

				float h = Utils::Max({ dx, dy }) + (m_sqrt2 - 1) * Utils::Min({ dx, dy });

				PathNode pathNode;

				pathNode.s_gridPos = pos;

				pathNode.s_g = m_nodePool[currentIndex].s_g + g;

				pathNode.s_h = h;

				pathNode.s_f = pathNode.s_g + pathNode.s_h;

				pathNode.s_previousPathNode = currentIndex;

				m_nodePool.push_back(pathNode);

				int newIndex = (int)m_nodePool.size() - 1;

				toExplore.push(newIndex);
			}
		}
		m_neighborPos.clear();
	}

	return path;
}

void Pathfinder::GetNeighborPos(Vector2i p_gridDimension, Vector2i p_startPos)
{
	for (int i = p_startPos.GetX() - 1; i <= p_startPos.GetX() + 1; i++)
	{
		for (int j = p_startPos.GetY() - 1; j <= p_startPos.GetY() + 1; j++)
		{
			if (i >= 0 && i < p_gridDimension.GetX() && j >= 0 && j < p_gridDimension.GetY())
			{
				if (i == p_startPos.GetX() && j == p_startPos.GetY())
					continue;

				m_neighborPos.push_back({ i, j });
			}
		}
	}
}

float Pathfinder::StepCost(Vector2i p_previousPos, Vector2i p_currentPos)
{
	if (p_previousPos == p_currentPos)
		return 0.f;

	Vector2i diffPos = p_currentPos - p_previousPos;

	if (diffPos.GetX() != 0 && diffPos.GetY() != 0)
		return std::sqrt(2);

	return 1.f;
}