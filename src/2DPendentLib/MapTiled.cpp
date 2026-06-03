#include "pch.h"
#include "MapTiled.h"
#include "GroupTiled.h"
#include "LayerTiled.h"
#include "PropertyTiled.h"
#include "ObjectGroupTiled.h"
#include "TilesetTiled.h"
#include "ImageLayerTiled.h"
#include "Render.h"
#include "AssetManager.h"
#include "Camera.h"
#include "Window.h"

//Private
void MapTiled::LoadXML(const std::string& p_path, Render* p_render)
{
    tinyxml2::XMLDocument document;
    OpenXML(p_path, document);

    tinyxml2::XMLElement* map = document.FirstChildElement("map");
    ReadMap(map);

    if (!map)
    {
        return;
    }
    tinyxml2::XMLElement* mapElement = map->FirstChildElement();

    while (mapElement)
    {
        std::string tagNameMap = mapElement->Value();

        if (tagNameMap == "tileset")
        {
            ReadTileset(mapElement, p_render);
        }
        else if (tagNameMap == "group")
        {
            ReadGroup(mapElement);
        }
        else if (tagNameMap == "layer")
        {
            ReadLayer(mapElement);
        }
        else if (tagNameMap == "properties")
        {
            ReadMapProperties(mapElement);
        }
        else if (tagNameMap == "objectgroup")
        {
            ReadObjectGroup(mapElement);
        }
        else if (tagNameMap == "imagelayer")
        {
            ReadImageLayer(mapElement);
        }
        mapElement = mapElement->NextSiblingElement();
    }
}

void MapTiled::OpenXML(const std::string& p_path, tinyxml2::XMLDocument& p_document)
{
    if (p_document.LoadFile(p_path.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Erreur chargement TMX: " << p_path << std::endl;
        return;
    }
}

void MapTiled::ReadMap(tinyxml2::XMLElement* p_map)
{
    if (!p_map)
    {
        std::cerr << "Erreur : balise <map> introuvable\n";
        return;
    }
    m_MapOrientation = XMLSafe::GetString(p_map, "orientation");
    m_MapRenderOrder = XMLSafe::GetString(p_map, "renderorder");

    m_MapInfinite = XMLSafe::GetBool(p_map, "infinite", false);

    std::string valueStr = XMLSafe::GetString(p_map, "backgroundcolor");
    std::string colorStr = valueStr.substr(1);  // Enlève #

    if (colorStr.size() == 8)
    {  // #AARRGGBB
        try
        {
            uint32_t hex = std::stoul(colorStr, nullptr, 16);
            m_MapBackgroundcolor = Color(
                (hex >> 16) & 0xFF,  // R
                (hex >> 8) & 0xFF,  // G  
                hex & 0xFF    // B
            );
        }
        catch (...)
        {
            m_MapBackgroundcolor = ColorsNamed::White;
        }
    }
    else if (colorStr.size() == 6)
    {  // #RRGGBB
        try
        {
            uint32_t hex = std::stoul(colorStr, nullptr, 16);
            m_MapBackgroundcolor = Color(
                (hex >> 16) & 0xFF,  // R
                (hex >> 8) & 0xFF,  // G  
                hex & 0xFF,   // B
                255                 // Alpha=opaque
            );
        }
        catch (...)
        {
            m_MapBackgroundcolor = ColorsNamed::White;
        }
    }
    else
    {
        m_MapBackgroundcolor = ColorsNamed::White;
    }

    m_MapNextLayerId = XMLSafe::GetInt(p_map, "nextlayerid", 1);
    m_MapNextObjectId = XMLSafe::GetInt(p_map, "nextobjectid", 1);

    m_MapDimension = { XMLSafe::GetInt(p_map, "width"), XMLSafe::GetInt(p_map, "height") };
    m_TileDimension = { XMLSafe::GetInt(p_map, "tilewidth"), XMLSafe::GetInt(p_map, "tileheight") };
}
void MapTiled::ReadMapProperties(tinyxml2::XMLElement* p_mapElement)
{
    tinyxml2::XMLElement* propertiesElement = p_mapElement->FirstChildElement();
    while (propertiesElement)
    {
        std::string tagNameProperties = propertiesElement->Value();
        if (tagNameProperties == "property")
        {
            PropertyTiled* tempProperty = new PropertyTiled();
            tempProperty->m_PropertyName = propertiesElement->Attribute("name");
            tempProperty->m_PropertyType = propertiesElement->Attribute("type");
            std::string valueStr = propertiesElement->Attribute("value");

            tempProperty->ReadPropertyValue(valueStr);

            m_properties.push_back(tempProperty);
        }
        propertiesElement = propertiesElement->NextSiblingElement();
    }
}

void MapTiled::ReadTileset(tinyxml2::XMLElement* p_mapElement, Render* p_render)
{
    TilesetTiled* tempTileset = new TilesetTiled();
    tempTileset->m_baliseTileset.s_TilesetFirstgid = XMLSafe::GetInt(p_mapElement, "firstgid");
    tempTileset->m_baliseTileset.s_TilesetSource = XMLSafe::GetString(p_mapElement, "source");
    tempTileset->LoadTSX(p_render);
    m_tilesets.push_back(tempTileset);
}

void MapTiled::ReadGroup(tinyxml2::XMLElement* p_mapElement)
{
    GroupTiled* tempGroupTiled = new GroupTiled();
    tempGroupTiled->ReadGroup(p_mapElement, this);
    m_groups.push_back(tempGroupTiled);
}

void MapTiled::ReadLayer(tinyxml2::XMLElement* p_mapElement)
{
    LayerTiled* tempLayerTiled = new LayerTiled();
    tempLayerTiled->ReadLayer(p_mapElement, this);

    tinyxml2::XMLElement* layerElement = p_mapElement->FirstChildElement();
    while (layerElement)
    {
        std::string tagNameLayer = layerElement->Value();
        if (tagNameLayer == "data")
        {
            tempLayerTiled->ReadData(layerElement);
        }
        else if (tagNameLayer == "properties")
        {
            tempLayerTiled->ReadLayerProperties(layerElement);
        }
        layerElement = layerElement->NextSiblingElement();
    }
    m_layers.push_back(tempLayerTiled);
}

void MapTiled::ReadObjectGroup(tinyxml2::XMLElement* p_mapElement)
{
    ObjectGroupTiled* tempObjectGroupTiled = new ObjectGroupTiled();
    tempObjectGroupTiled->ReadObjectGroup(p_mapElement);

    tinyxml2::XMLElement* objectGroupElement = p_mapElement->FirstChildElement();
    while (objectGroupElement)
    {
        std::string tagNameObjectGroup = objectGroupElement->Value();
        if (tagNameObjectGroup == "object")
        {
            tempObjectGroupTiled->ReadObject(objectGroupElement, this);
        }
        else if (tagNameObjectGroup == "properties")
        {
            tempObjectGroupTiled->ReadObjectGroupProperties(objectGroupElement);
        }
        objectGroupElement = objectGroupElement->NextSiblingElement();
    }
    m_objectGroups.push_back(tempObjectGroupTiled);
}

void MapTiled::ReadImageLayer(tinyxml2::XMLElement* p_mapElement)
{
    ImageLayerTiled* tempImageLayerTiled = new ImageLayerTiled();
    tempImageLayerTiled->ReadImageLayer(p_mapElement);

    tinyxml2::XMLElement* imageLayerElement = p_mapElement->FirstChildElement();
    while (imageLayerElement)
    {
        std::string tagNameImageLayer = imageLayerElement->Value();
        if (tagNameImageLayer == "image")
        {
            tempImageLayerTiled->m_ImageLayerImage->ReadImage(imageLayerElement);
        }
        else if (tagNameImageLayer == "properties")
        {
            tempImageLayerTiled->ReadImageLayerProperties(imageLayerElement);
        }
        imageLayerElement = imageLayerElement->NextSiblingElement();
    }
    m_imageLayers.push_back(tempImageLayerTiled);
}

//Public
MapTiled::MapTiled(const std::string& p_path, Render* p_render)
{
    m_groups.reserve(8);        // groupes au niveau racine de la map
    m_layers.reserve(12);        // layers de tuiles principaux
    m_tilesets.reserve(6);      // rarement plus de 6 tilesets par map
    m_properties.reserve(8);    // propriétés custom de la map
    m_objectGroups.reserve(8);  // groupes d'objets
    m_imageLayers.reserve(4);   // image layers

	m_render = p_render;
    LoadXML(p_path, m_render);
}

MapTiled::~MapTiled()
{
    for (GroupTiled* group : m_groups)
    {
        delete group;
    }
    m_groups.clear();
    for (LayerTiled* layer : m_layers)
    {
        delete layer;
    }
    m_layers.clear();
    for (TilesetTiled* tileset : m_tilesets)
    {
        delete tileset;
    }
    m_tilesets.clear();
    for (PropertyTiled* property : m_properties)
    {
        delete property;
    }
    m_properties.clear();
    for (ObjectGroupTiled* objectGroup : m_objectGroups)
    {
        delete objectGroup;
    }
    m_objectGroups.clear();
    for (ImageLayerTiled* imageLayer : m_imageLayers)
    {
        delete imageLayer;
    }
	m_imageLayers.clear();
}

void MapTiled::Draw(Render* f_render, Window* f_window, Camera* p_camera)
{

    if (m_tilesets.empty())
    {
        //std::cerr << "Pas de tileset pour draw" << std::endl;
        return;
    }
    int countTexture = 0;
    for (size_t i = 0; i < m_tilesets.size(); i++)
    {
        if (m_tilesets[i]->GetIdTilesetTexture() == "empty")
        {
            //std::cerr << "Pas de texture pour ce tileset :" << m_tilesets[i] << std::endl;
        }
        else
        {
            countTexture++;
        }
    }
    if (countTexture == 0)
    {
        //std::cerr << "Pas de texture pour draw" << std::endl;
        return;
    }

    for (LayerTiled* layerTiled : m_layers)
    {
        if (layerTiled->m_LayerVisible == false)
        {
            continue;
        }

        float posTileX;
        float posTileY;

        Vector2i dimensionWindow = f_window->GetWindowSize();

        // Parcours de chaque ligne de la layer
        for (int y = 0; y < layerTiled->m_LayerDimension.GetY(); y++)
        {
            posTileY = y * m_TileDimension.GetY();

            // Parcours de chaque colonne de la layer
            for (int x = 0; x < layerTiled->m_LayerDimension.GetX(); x++)
            {
                posTileX = x * m_TileDimension.GetX();

                Vector2f screenPosTopLeft = p_camera->WorldToScreen<float>({ posTileX, posTileY });
                Vector2f screenPosBottomRight = p_camera->WorldToScreen<float>({ posTileX + m_TileDimension.GetX(), posTileY + m_TileDimension.GetY() });

                if (screenPosBottomRight.GetX() < 0 || screenPosTopLeft.GetX() > dimensionWindow.GetX() ||
                    screenPosBottomRight.GetY() < 0 || screenPosTopLeft.GetY() > dimensionWindow.GetY())
                {
                    //std::cerr << "no draw" << std::endl;
                    continue;
                }

                 // Calcul de l'index dans le vector 1D à partir de coordonnées 2D (x,y)
                 // Ex: x=2, y=3, largeur de la layer = 10 => index = 3*10 + 2 = 32
                 // Cela permet de convertir les coordonnées 2D (x,y) en un index pour accéder au vector 1D s_LayerTiles
                int index = y * layerTiled->m_LayerDimension.GetX() + x;

                // Récupération du tileID dans le vector s_LayerTiles
                // Ex: index = 32 => tileNumber = s_LayerTiles[32] = 6
                // tileNumber correspond au GID de la tile dans le tileset
                int tile_gid = layerTiled->m_LayerTiles[index];

                // Si tileNumber == 0, la case est vide => on ne dessine rien
                if (tile_gid == 0) continue;

                TilesetTiled* tempTileset = nullptr;
                for (auto& ts : m_tilesets)
                {
                    if (tile_gid < ts->m_baliseTileset.s_TilesetFirstgid)
                    {
                        continue;
                    }
                    else
                    {
                        tempTileset = ts;
                    }
                }
                if (!tempTileset)
                {
                    continue;
                }
                if (tempTileset->GetIdTilesetTexture() == "empty")
                {
                    continue;
                }

                // Calcul de la position de la tile dans le tileset
                int localID = tile_gid - tempTileset->m_baliseTileset.s_TilesetFirstgid; // GID local (commence à 0)

                // CORRIGÉ : Calcul des coordonnées UV dans le tileset
                // Ex: localID = 6, tilesPerRow = 4
                // tu = 6 % 4 = 2 (colonne dans le tileset)
                // tv = 6 / 4 = 1 (ligne dans le tileset)
                // tu et tv sont les coordonnées de la tile dans le tileset, exprimées en pixels
                int tu, tv;
                if (tempTileset->m_TilesetColumns == 0) {
                    // Tile invalide : skip ou tuile vide
                    tu = 0; tv = 0;
                }
                else {
                    tu = (localID % tempTileset->m_TilesetColumns) * m_TileDimension.GetX();
                    tv = (localID / tempTileset->m_TilesetColumns) * m_TileDimension.GetY();
                }

                SDL_Rect srcRect = {
                    tu,                        // left
                    tv,                        // top
                    m_TileDimension.GetX(),    // width
                    m_TileDimension.GetY()     // height
                };

                // Position de la tile sur la fenêtre

                Vector2i postile = p_camera->WorldToScreen<int>({ x * m_TileDimension.GetX(), y * m_TileDimension.GetY() });
                SDL_Rect dstRect = {
                    postile.GetX(),    // pos X à l'écran
                    postile.GetY(),    // pos Y à l'écran
                    m_TileDimension.GetX() * p_camera->GetZoom(),        // width
                    m_TileDimension.GetY() * p_camera->GetZoom()         // height
                };
                // Dessiner la tile
                if (SDL_RenderCopy(
                    f_render->m_renderer,
                    AssetManager::Get()->GetTexture(tempTileset->GetIdTilesetTexture()),
                    &srcRect,
                    &dstRect
                ) < 0)
                {
                    SDL_Log("Lib/MapTiled.cpp/Draw/SDL_RenderCopy/cela na pas fonctionné : %s", SDL_GetError());
                    return;
                }
            }
        }
    }
}

PropertyTiled* MapTiled::GetMapsProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}

GroupTiled* MapTiled::GetGroupByName(const std::string& p_name)
{
    for (GroupTiled* group : m_groups)
    {
        if (group->m_GroupName == p_name)
        {
            return group;
        }
	}
}

LayerTiled* MapTiled::GetLayerByName(const std::string& p_name)
{
    for (LayerTiled* layer : m_layers)
    {
        if (layer->m_LayerName == p_name)
        {
            return layer;
        }
	}
}

TilesetTiled* MapTiled::GetTilesetByName(const std::string& p_name)
{
    for (TilesetTiled* tileset : m_tilesets)
    {
        if (tileset->m_TilesetName == p_name)
        {
            return tileset;
        }
    }
}

ObjectGroupTiled* MapTiled::GetObjectGroupByName(const std::string& p_name)
{
    for (ObjectGroupTiled* objectGroup : m_objectGroups)
    {
        if (objectGroup->m_ObjectGroupName == p_name)
        {
            return objectGroup;
        }
	}
}

ImageLayerTiled* MapTiled::GetImageLayerByName(const std::string& p_name)
{
    for (ImageLayerTiled* imageLayer : m_imageLayers)
    {
        if (imageLayer->m_ImageLayerName == p_name)
        {
            return imageLayer;
        }
    }
}