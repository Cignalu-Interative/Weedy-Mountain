#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "tinyxml2.h"
#include "PropertyTiled.h"
#include "Sprite.h"

class MapTiled;
class TileTiled;
class TilesetTiled
{
private:

	std::string m_TilesetName;
	std::string m_TilesetClass;

	struct BaliseTileset
	{
		int s_TilesetFirstgid;
		std::string s_TilesetSource;
	};
	BaliseTileset m_baliseTileset;

	std::string m_TilesetBackgroundColor;
	std::string m_TilesetObjectAlignment;

	struct BaliseGrid
	{
		std::string m_TilesetOrientation;
		Vector2i m_TilesetGridDimension;
	};
	BaliseGrid m_baliseGrid;

	struct BaliseImage
	{
		std::string s_ImagePath;
		Vector2i s_Dimension;
	};
	BaliseImage m_baliseImage;

	std::string m_IdTilesetTexture = "empty";

	std::vector<TileTiled*> m_PropertiesTiles;
	std::vector<PropertyTiled*> m_properties;

	Vector2i m_TilesetTileDimension;

	struct BaliseTileOffset
	{
		Vector2i m_TilesetTileOffset;
	};
	BaliseTileOffset m_baliseTileOffset;

	int m_TilesetSpacing;
	int m_TilesetMargin;
	int m_TilesetTileCount;
	int m_TilesetColumns;

	struct BaliseTransformations
	{
		bool m_TilesetHorizontalFlip;
		bool m_TilesetVerticalFlip;
		bool m_TilesetRotateFlip;
		bool m_TilesetPreferunTransformedTiles;
	};
	BaliseTransformations m_baliseTransformations;

	void LoadTSX(Render* p_render);

	void OpenTSX(tinyxml2::XMLDocument& p_document);

	void ReadTileset(tinyxml2::XMLElement* p_tilesetElement);
	void ReadTilesetProperties(tinyxml2::XMLElement* p_tilesetElement);

	void ReadTileOffset(tinyxml2::XMLElement* p_tilesetElement);

	void ReadGrid(tinyxml2::XMLElement* p_tilesetElement);

	void ReadTransformations(tinyxml2::XMLElement* p_tilesetElement);

	void ReadImage(tinyxml2::XMLElement* p_tilesetElement);

	void ReadTile(tinyxml2::XMLElement* p_tilesetElement);

	void LoadTilesetTexture(Render* p_render);

	void SetIdTilesetTexture(const std::string& p_id) { m_IdTilesetTexture = p_id; }

public:

	friend class MapTiled;

	TilesetTiled();
	~TilesetTiled();

	PropertyTiled* GetTilesetProperty(const std::string& p_propertyName) const;

	int GetTilesetFirstgid() const { return m_baliseTileset.s_TilesetFirstgid; }
	std::string GetTilesetSource() const { return m_baliseTileset.s_TilesetSource; }

	std::string GetTilesetName() const { return m_TilesetName; }
	std::string GetTilesetClass() const { return m_TilesetClass; }
	Vector2i GetTilesetTileDimension() const { return m_TilesetTileDimension; }
	int GetTilesetSpacing() const { return m_TilesetSpacing; }
	int GetTilesetMargin() const { return m_TilesetMargin; }
	int GetTilesetTileCount() const { return m_TilesetTileCount; }
	int GetTilesetColumns() const { return m_TilesetColumns; }
	std::string GetTilesetBackgroundColor() const { return m_TilesetBackgroundColor; }
	std::string GetTilesetObjectAlignment() const { return m_TilesetObjectAlignment; }

	Vector2i GetTilesetTileOffset() const { return m_baliseTileOffset.m_TilesetTileOffset; }

	std::string GetTilesetOrientation() const { return m_baliseGrid.m_TilesetOrientation; }
	Vector2i GetTilesetGridDimension() const { return m_baliseGrid.m_TilesetGridDimension; }

	bool IsTilesetHorizontalFlip() const { return m_baliseTransformations.m_TilesetHorizontalFlip; }
	bool IsTilesetVerticalFlip() const { return m_baliseTransformations.m_TilesetVerticalFlip; }
	bool IsTilesetRotateFlip() const { return m_baliseTransformations.m_TilesetRotateFlip; }
	bool IsTilesetPreferunTransformedTiles() const { return m_baliseTransformations.m_TilesetPreferunTransformedTiles; }

	std::string GetImagePath() const { return m_baliseImage.s_ImagePath; }
	Vector2i GetImageDimension() const { return m_baliseImage.s_Dimension; }

	std::string GetIdTilesetTexture() const { return m_IdTilesetTexture; }

	std::vector<TileTiled*> GetAllPropertiesTiles() { return m_PropertiesTiles; }
	std::vector<PropertyTiled*> GetAllProperties() { return m_properties; }

	std::vector<TileTiled*> GetTileTiledByType(std::string p_type);
};