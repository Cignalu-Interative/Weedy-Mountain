#include "pch.h"
#include "Pathfinder.h"
#include <algorithm>

std::vector<Tile*> Pathfinder::FindPath(NavigationGrid& p_navigationGrid, Vector2i p_start, Vector2i p_goal)
{
    std::vector<Tile*> path;
    std::vector<PathNode*> toExplore;
    std::vector<PathNode*> toDelete;

    float g = 0.f;

    PathNode* previousNode = new PathNode;

    previousNode->s_gridPos = p_start;

    previousNode->s_g = g;

    float dx = std::abs(p_start.GetX() - p_goal.GetX());
    float dy = std::abs(p_start.GetY() - p_goal.GetY());

    previousNode->s_h = Utils::Max({ dx, dy }) + (std::sqrt(2) - 1) * Utils::Min({ dx, dy });

    previousNode->s_f = previousNode->s_g + previousNode->s_h;

    previousNode->s_previousPathNode = nullptr;
    std::vector<Vector2i> neighborPos;

    std::vector<Vector2i> alreadyExplore;

    toExplore.push_back(previousNode);
    toDelete.push_back(previousNode);

    while (toExplore.size() > 0)
    {
        int bestIndex = 0;
        float bestF = toExplore[0]->s_f;

        for (int i = 1; i < toExplore.size(); i++)
        {
            if (toExplore[i]->s_f < bestF)
            {
                bestF = toExplore[i]->s_f;
                bestIndex = i;
            }
        }

        PathNode* currentNode = toExplore[bestIndex];
        alreadyExplore.push_back(currentNode->s_gridPos);

        toExplore.erase(toExplore.begin() + bestIndex);



        if (currentNode->s_gridPos == p_goal)
        {
            PathNode* tempNode = currentNode;

            while (tempNode != nullptr)
            {
                path.push_back(p_navigationGrid.GetTileByGridPos(tempNode->s_gridPos));
                tempNode = tempNode->s_previousPathNode;
            }

            std::reverse(path.begin(), path.end());

            for (PathNode* deleted : toDelete)
            {
                delete deleted;
            }

            return path;
        }

        neighborPos = GetNeighborPos(p_navigationGrid.GetGridDimension(), currentNode->s_gridPos);

        for (Vector2i pos : neighborPos)
        {
            if (p_navigationGrid.GetTileByGridPos(pos)->canBeCrossed &&
                std::find(alreadyExplore.begin(), alreadyExplore.end(), pos) == alreadyExplore.end())
            {
                g = StepCost(currentNode->s_gridPos, pos);

                float dx = std::abs(pos.GetX() - p_goal.GetX());
                float dy = std::abs(pos.GetY() - p_goal.GetY());

                float h = Utils::Max({ dx, dy }) + (std::sqrt(2) - 1) * Utils::Min({ dx, dy });

                auto it = std::find_if(toExplore.begin(), toExplore.end(), [pos](PathNode* node) {return node->s_gridPos == pos; });

                if (it == toExplore.end())
                {
                    PathNode* pathNode = new PathNode;

                    pathNode->s_gridPos = pos;

                    pathNode->s_g = currentNode->s_g + g;

                    pathNode->s_h = h;

                    pathNode->s_f = pathNode->s_g + pathNode->s_h;

                    pathNode->s_previousPathNode = currentNode;

                    toExplore.push_back(pathNode);
                    toDelete.push_back(pathNode);
                }
                else
                {
                    if (currentNode->s_g + g < (*it)->s_g)
                    {
                        (*it)->s_g = currentNode->s_g + g;
                        (*it)->s_h = h;
                        (*it)->s_f = (*it)->s_g + (*it)->s_h;
                        (*it)->s_previousPathNode = currentNode;
                    }
                }
            }
        }
        neighborPos.clear();
    }

    for (PathNode* deleted : toDelete)
    {
        delete deleted;
    }

    return path;
}

std::vector<Vector2i> Pathfinder::GetNeighborPos(Vector2i p_gridDimension, Vector2i p_startPos)
{
    std::vector<Vector2i> allNeighbors;
    
    for (int i = p_startPos.GetX() - 1; i <= p_startPos.GetX() + 1; i++)
    {
        for (int j = p_startPos.GetY() - 1; j <= p_startPos.GetY() + 1; j++)
        {
            if (i >= 0 && i < p_gridDimension.GetX() && j >= 0 && j < p_gridDimension.GetY())
            {
                if (i == p_startPos.GetX() && j == p_startPos.GetY())
                    continue;

                allNeighbors.push_back({ i, j });
            }
        }
    }

    return allNeighbors;
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