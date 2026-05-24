#include "pch.h"
#include "LayerTiled.h"
#include "MapTiled.h"
#include "GroupTiled.h"

void LayerTiled::ReadLayer(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map)
{
    m_LayerId = p_map->m_MapNextLayerId++;
	m_LayerId = XMLSafe::GetInt(p_mapElement, "id");
	m_LayerName = XMLSafe::GetString(p_mapElement, "name");
	m_LayerClass = XMLSafe::GetString(p_mapElement, "class");
	m_LayerDimension = { XMLSafe::GetInt(p_mapElement, "width"), XMLSafe::GetInt(p_mapElement, "height") };
	m_LayerVisible = XMLSafe::GetBool(p_mapElement, "visible", true);
	m_LayerLocked = XMLSafe::GetBool(p_mapElement, "locked", false);
	m_LayerOpacity = XMLSafe::GetFloat(p_mapElement, "opacity", 1.0f);
	m_LayerTintColor = XMLSafe::GetString(p_mapElement, "tintcolor");
	m_LayerOffset = { XMLSafe::GetFloat(p_mapElement, "offsetx", 0.0f), XMLSafe::GetFloat(p_mapElement, "offsety", 0.0f) };
	m_LayerParallaxFactor = { XMLSafe::GetFloat(p_mapElement, "parallaxx", 1.0f), XMLSafe::GetFloat(p_mapElement, "parallaxy", 1.0f) };
}

void LayerTiled::ReadData(tinyxml2::XMLElement* p_layerElement)
{
    std::string csv = p_layerElement->GetText(); //Récupere les nombres sous forme de texte
    std::stringstream ss(csv); //Convertie du texte en nombre
    std::string item; // le contenue de la tuile (exemple 0 ou 124 ou 45)
    while (std::getline(ss, item, ','))
    {
        if (!item.empty())
        {
            try
            {
                m_LayerTiles.push_back(std::stoi(item));
            }
            catch (...)
            {
                m_LayerTiles.push_back(0);
            }
        }
        else
        {
            continue;
        }
    }
}

void LayerTiled::ReadLayerProperties(tinyxml2::XMLElement* p_layerElement)
{
    tinyxml2::XMLElement* propertiesElement = p_layerElement->FirstChildElement();
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

LayerTiled::LayerTiled()
{
    m_LayerTiles.reserve(m_LayerDimension.GetX() * m_LayerDimension.GetY()); // taille exacte de la grille
    m_properties.reserve(8);
}

LayerTiled::~LayerTiled()
{
    for (PropertyTiled* prop : m_properties)
    {
        delete prop;
	}
	m_properties.clear();
}

PropertyTiled* LayerTiled::GetLayerProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}