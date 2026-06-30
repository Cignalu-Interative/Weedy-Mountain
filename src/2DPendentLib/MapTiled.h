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

// Struct de retour pour les requêtes de tiles
struct TileInfo
{
	int gid = 0;
	Vector2i gridPos;               // coordonnées grille (colonne, ligne)
	Vector2f worldPos;              // coin haut-gauche en pixels
	TilesetTiled* tileset = nullptr;
	TileTiled* tileTiled = nullptr;   // nullptr si pas de propriétés custom
	LayerTiled* layer = nullptr;

	bool IsEmpty() const { return gid == 0; }
	bool IsValid() const { return layer != nullptr && !IsEmpty(); }
};

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

	std::vector<TilesetTiled*> m_gidToTileset;

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

	void BuildGidLookupTable();

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

	// Conversions coordonnées 
	Vector2i WorldToGrid(Vector2f p_worldPos) const;
	Vector2f GridToWorld(Vector2i p_gridPos) const;

	// Accès à une tile
	// p_layerName = "" -> cherche dans tous les layers, retourne le premier GID non nul
	TileInfo GetTileAtWorld(Vector2f p_worldPos, const std::string& p_layerName = "") const;
	TileInfo GetTileAtGrid(Vector2i p_gridPos, const std::string& p_layerName = "") const;

	// Propriétés d'une tile 
	PropertyTiled* GetTilePropertyAtWorld(Vector2f p_worldPos, const std::string& p_propertyName, const std::string& p_layerName = "") const;
	PropertyTiled* GetTilePropertyAtGrid(Vector2i p_gridPos, const std::string& p_propertyName, const std::string& p_layerName = "") const;
	bool TileHasPropertyAtWorld(Vector2f p_worldPos, const std::string& p_propertyName, const std::string& p_layerName = "") const;
	bool TileHasPropertyAtGrid(Vector2i p_gridPos, const std::string& p_propertyName, const std::string& p_layerName = "") const;

	// Recherche par propriété 
	std::vector<TileInfo> GetAllTilesWithProperty(const std::string& p_propertyName, const std::string& p_layerName = "") const;

	// Helpers internes
	TilesetTiled* GetTilesetForGID(int p_gid)  const;
	TileTiled* GetTileTiledForGID(int p_gid) const;

	// Placement d'entités
	// Retourne les positions monde (coin haut-gauche) de toutes les tiles ayant la propriété
	std::vector<Vector2f> GetWorldPositionsWithProperty(const std::string& p_propertyName, const std::string& p_layerName = "") const;
};