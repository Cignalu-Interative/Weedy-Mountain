#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Vector2.h"
#include "Transformable.h"
#include "tinyxml2.h"
#include "Sprite.h"
#include "ObjectTiled.h"
#include "XMLSafe.h"
#include "TileTiled.h"
//#include "Camera.h"	
#include "Utils.h"

class GroupTiled;
class LayerTiled;
class TilesetTiled;
class PropertyTiled;
class ObjectGroupTiled;
class ImageLayerTiled;
class MapTiled : public Transformable
{
private:

	std::string m_MapOrientation;
	std::string m_MapRenderOrder;

	Vector2i m_MapDimension;
	Vector2i m_TileDimension;

	std::vector<GroupTiled*> m_groups;
	std::vector<LayerTiled*> m_layers;
	std::vector<TilesetTiled*> m_tilesets;
	std::vector<PropertyTiled*> m_properties;
	std::vector<ObjectGroupTiled*> m_objectGroups;
	std::vector<ImageLayerTiled*> m_imageLayers;

	int m_MapNextLayerId;
	int m_MapNextObjectId;

	Color m_MapBackgroundcolor;

	bool m_MapInfinite;

	Render* m_render;

	void LoadXML(const std::string& p_path, Render* p_render);

	void OpenXML(const std::string& p_path, tinyxml2::XMLDocument& p_document);

	void ReadMap(tinyxml2::XMLElement* p_mapElement);
	void ReadMapProperties(tinyxml2::XMLElement* p_mapElement);

	void ReadTileset(tinyxml2::XMLElement* p_mapElement, Render* p_render);

	void ReadGroup(tinyxml2::XMLElement* p_mapElement);

	void ReadLayer(tinyxml2::XMLElement* p_mapElement);

	void ReadObjectGroup(tinyxml2::XMLElement* p_mapElement);

	void ReadImageLayer(tinyxml2::XMLElement* p_mapElement);

public:

	friend class ObjectGroupTiled;
	friend class ObjectTiled;
	friend class LayerTiled;

	MapTiled(const std::string& p_path, Render* p_render);
	~MapTiled();

	void Draw(Render* f_render, Window* f_window, Camera* p_camera);

	/*void DrawMap();*/

	///////////////////////////////////////////////::

	PropertyTiled* GetMapsProperty(const std::string& p_propertyName) const;

	std::string GetMapOrientation() const { return m_MapOrientation; }
	std::string GetMapRenderOrder() const { return m_MapRenderOrder; }

	Vector2i GetMapDimension() const { return m_MapDimension; }
	Vector2i GetTileDimension() const { return m_TileDimension; }

	bool IsMapInfinite() const { return m_MapInfinite; }

	int GetMapNextLayerId() const { return m_MapNextLayerId; }
	int GetMapNextObjectId() const { return m_MapNextObjectId; }

	Color GetMapBackgroundColor() const { return m_MapBackgroundcolor; }

	std::vector<GroupTiled*> GetAllGroups() { return m_groups; }
	std::vector<LayerTiled*> GetAllLayers() { return m_layers; }
	std::vector<TilesetTiled*> GetAllTilesets() { return m_tilesets; }
	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }
	std::vector<ObjectGroupTiled*> GetAllObjectGroups() { return m_objectGroups; }
	std::vector<ImageLayerTiled*> GetAllImageLayers() { return m_imageLayers; }

	GroupTiled* GetGroupByName(const std::string& p_name);
	LayerTiled* GetLayerByName(const std::string& p_name);
	TilesetTiled* GetTilesetByName(const std::string& p_name);
	ObjectGroupTiled* GetObjectGroupByName(const std::string& p_name);
	ImageLayerTiled* GetImageLayerByName(const std::string& p_name);
};