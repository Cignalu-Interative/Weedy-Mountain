#pragma once
#include <string>
#include <vector>
#include "2DPendentLib/Vector2.h"
#include "2DPendentLib/tinyxml2.h"
#include "PropertyTiled.h"
#include "XMLSafe.h"

class TileTiled
{
private:

	std::string m_TileType;

	std::vector<PropertyTiled*> m_properties;
	
	int m_TileId;
	int m_TileProbability;

	void ReadTile(tinyxml2::XMLElement* p_tileElement);

	void ReadTileProperties(tinyxml2::XMLElement* p_tileElement);

public:

	friend class TilesetTiled;
	friend class MapTiled;

	TileTiled();
	~TileTiled();

	PropertyTiled* GetTileProperty(const std::string& p_propertyName) const;
};

