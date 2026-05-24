#include "pch.h"
#include "TilesetTiled.h"
#include "MapTiled.h"
#include "2DPendentLib/tinyxml2.h"
#include "XMLSafe.h"
#include "TileTiled.h"
#include "Render.h"
#include "AssetManager.h"
#include "Utils.h"

void TilesetTiled::LoadTSX(Render* p_render)
{
    tinyxml2::XMLDocument document;
	OpenTSX(document);

    tinyxml2::XMLElement* tileset = document.FirstChildElement("tileset");
    ReadTileset(tileset);

    if (!tileset)
    {
        return;
    }

	tinyxml2::XMLElement* tilesetElement = tileset->FirstChildElement();
    while (tilesetElement)
    {
        std::string tagNameTileset = tilesetElement->Value();

        if (tagNameTileset == "image")
        {
            ReadImage(tilesetElement);
		}
        else if (tagNameTileset == "tileoffset")
        {
            ReadTileOffset(tilesetElement);
        }
        else if (tagNameTileset == "grid")
        {
            ReadGrid(tilesetElement);
        }
        else if (tagNameTileset == "transformations")
        {
            ReadTransformations(tilesetElement);
		}
        else if (tagNameTileset == "properties")
        {
            ReadTilesetProperties(tilesetElement);
		}
        else if (tagNameTileset == "tile")
        {
			ReadTile(tilesetElement);
        }
        tilesetElement = tilesetElement->NextSiblingElement();
    }
    LoadTilesetTexture(p_render);
}

void TilesetTiled::OpenTSX(tinyxml2::XMLDocument& p_document)
{
    int errorid = p_document.LoadFile(m_baliseTileset.s_TilesetSource.c_str());
    if (errorid != 0)
    {
        std::cerr << errorid << std::endl;
    }
}

void TilesetTiled::ReadTileset(tinyxml2::XMLElement* p_tilesetElement)
{
    m_TilesetName = XMLSafe::GetString(p_tilesetElement, "name");
	m_TilesetClass = XMLSafe::GetString(p_tilesetElement, "class");
    m_TilesetTileDimension = { XMLSafe::GetInt(p_tilesetElement, "tilewidth"), XMLSafe::GetInt(p_tilesetElement, "tileheight") };
	m_TilesetSpacing = XMLSafe::GetInt(p_tilesetElement, "spacing");
	m_TilesetMargin = XMLSafe::GetInt(p_tilesetElement, "margin");
    m_TilesetTileCount = XMLSafe::GetInt(p_tilesetElement, "tilecount");
    m_TilesetColumns = XMLSafe::GetInt(p_tilesetElement, "columns");
	m_TilesetBackgroundColor = XMLSafe::GetString(p_tilesetElement, "backgroundcolor");
	m_TilesetObjectAlignment = XMLSafe::GetString(p_tilesetElement, "objectalignment");
}
void TilesetTiled::ReadTilesetProperties(tinyxml2::XMLElement* p_tilesetElement)
{
    tinyxml2::XMLElement* propertiesElement = p_tilesetElement->FirstChildElement();
    while (propertiesElement)
    {
        std::string tagNameProperties = propertiesElement->Value();
        if (tagNameProperties == "property")
        {
            PropertyTiled* tempProperty = new PropertyTiled();
            tempProperty->m_PropertyName = XMLSafe::GetString(propertiesElement, "name");
            tempProperty->m_PropertyType = XMLSafe::GetString(propertiesElement, "type");
            std::string valueStr = XMLSafe::GetString(propertiesElement, "value");

            tempProperty->ReadPropertyValue(valueStr);

            m_properties.push_back(tempProperty);
        }
        propertiesElement = propertiesElement->NextSiblingElement();
    }
}
void TilesetTiled::ReadTileOffset(tinyxml2::XMLElement* p_tilesetElement)
{
    m_baliseTileOffset.m_TilesetTileOffset = { XMLSafe::GetInt(p_tilesetElement, "x"), XMLSafe::GetInt(p_tilesetElement, "y") };
}
void TilesetTiled::ReadGrid(tinyxml2::XMLElement* p_tilesetElement)
{
    m_baliseGrid.m_TilesetOrientation = XMLSafe::GetString(p_tilesetElement, "orientation");
	m_baliseGrid.m_TilesetGridDimension = { XMLSafe::GetInt(p_tilesetElement, "width"), XMLSafe::GetInt(p_tilesetElement, "height") };
}
void TilesetTiled::ReadTransformations(tinyxml2::XMLElement* p_tilesetElement)
{
    m_baliseTransformations.m_TilesetHorizontalFlip = XMLSafe::GetBool(p_tilesetElement, "hflip");
    m_baliseTransformations.m_TilesetVerticalFlip = XMLSafe::GetBool(p_tilesetElement, "vflip");
    m_baliseTransformations.m_TilesetRotateFlip = XMLSafe::GetBool(p_tilesetElement, "rotate");
    m_baliseTransformations.m_TilesetPreferunTransformedTiles = XMLSafe::GetBool(p_tilesetElement, "preferuntransformed");
}
void TilesetTiled::ReadImage(tinyxml2::XMLElement* p_imageElement)
{
    m_baliseImage.s_ImagePath = XMLSafe::GetString(p_imageElement, "source");
    m_baliseImage.s_Dimension = { XMLSafe::GetInt(p_imageElement, "width"), XMLSafe::GetInt(p_imageElement, "height") };
}
void TilesetTiled::ReadTile(tinyxml2::XMLElement* p_tileElement)
{
    TileTiled* tempTileTiled = new TileTiled();
    tempTileTiled->ReadTile(p_tileElement);

    tinyxml2::XMLElement* tileElement = p_tileElement->FirstChildElement();
    while (tileElement)
    {
        std::string tagNameTile = tileElement->Value();

        if (tagNameTile == "properties")
        {
            tempTileTiled->ReadTileProperties(tileElement);
        }
        tileElement = tileElement->NextSiblingElement();
    }
    m_PropertiesTiles.push_back(tempTileTiled);
}

void TilesetTiled::LoadTilesetTexture(Render* p_render)
{
	m_IdTilesetTexture = Utils::GetNameFromPath(m_baliseImage.s_ImagePath);
	AssetManager::Get()->AddTextureByPath(m_baliseImage.s_ImagePath);
}

TilesetTiled::TilesetTiled()
{
    m_PropertiesTiles.reserve(m_TilesetTileCount); // taille exacte connue à l'avance
    m_properties.reserve(8);                        // propriétés custom du tileset
}

TilesetTiled::~TilesetTiled()
{
    for (TileTiled* tile : m_PropertiesTiles)
    {
        delete tile;
    }
    m_PropertiesTiles.clear();
    for (PropertyTiled* property : m_properties)
    {
        delete property;
    }
	m_properties.clear();
}

PropertyTiled* TilesetTiled::GetTilesetProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}

std::vector<TileTiled*> TilesetTiled::GetTileTiledByType(std::string p_type)
{
	std::vector<TileTiled*> tilesOfType;

    for (TileTiled* tile : m_PropertiesTiles)
    {
        if (tile->m_TileType == p_type)
        {
            tilesOfType.push_back(tile);
        }
	}
	return tilesOfType;
}