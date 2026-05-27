#include "pch.h"
#include "GroupTiled.h"
#include "MapTiled.h"
#include "LayerTiled.h"
#include "ObjectGroupTiled.h"
#include "ImageLayerTiled.h"

void GroupTiled::ReadGroup(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map)
{
	m_GroupId = XMLSafe::GetString(p_mapElement, "id");
	m_GroupName = XMLSafe::GetString(p_mapElement, "name");
	m_GroupClass = XMLSafe::GetString(p_mapElement, "class");
	m_GroupVisible = XMLSafe::GetBool(p_mapElement, "visible", true);
	m_GroupLocked = XMLSafe::GetBool(p_mapElement, "locked", false);
	m_GroupOpacity = XMLSafe::GetFloat(p_mapElement, "opacity", 1.0f);
	m_GroupTintColor = XMLSafe::GetString(p_mapElement, "tintcolor");
	m_GroupOffset = { XMLSafe::GetFloat(p_mapElement, "offsetx", 0.0f), XMLSafe::GetFloat(p_mapElement, "offsety", 0.0f) };
	m_GroupParallaxFactor = { XMLSafe::GetFloat(p_mapElement, "parallaxx", 1.0f), XMLSafe::GetFloat(p_mapElement, "parallaxy", 1.0f) };

	ReadGroupElement(p_mapElement, p_map);
}

void GroupTiled::ReadLayer(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map)
{
    LayerTiled* tempLayerTiled = new LayerTiled();
    tempLayerTiled->ReadLayer(p_mapElement, p_map);

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

void GroupTiled::ReadObjectGroup(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map)
{
    ObjectGroupTiled* tempObjectGroupTiled = new ObjectGroupTiled();
    tempObjectGroupTiled->ReadObjectGroup(p_mapElement);

    tinyxml2::XMLElement* objectGroupElement = p_mapElement->FirstChildElement();
    while (objectGroupElement)
    {
        std::string tagNameObjectGroup = objectGroupElement->Value();
        if (tagNameObjectGroup == "object")
        {
            tempObjectGroupTiled->ReadObject(objectGroupElement, p_map);
        }
        else if (tagNameObjectGroup == "properties")
        {
            tempObjectGroupTiled->ReadObjectGroupProperties(objectGroupElement);
        }
        objectGroupElement = objectGroupElement->NextSiblingElement();
    }
    m_objectGroups.push_back(tempObjectGroupTiled);
}
void GroupTiled::ReadImageLayer(tinyxml2::XMLElement* p_mapElement)
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

void GroupTiled::ReadGroupElement(tinyxml2::XMLElement* p_mapElement, MapTiled* p_map)
{
    tinyxml2::XMLElement* GroupElement = p_mapElement->FirstChildElement();
    while (GroupElement)
    {
        std::string tagNameGroup = GroupElement->Value();

        if (tagNameGroup == "properties")
        {
            ReadGroupProperties(GroupElement);
        }
        else if (tagNameGroup == "layer")
        {
            ReadLayer(GroupElement, p_map);
        }
        else if (tagNameGroup == "objectgroup")
        {
            ReadObjectGroup(GroupElement, p_map);
        }
        else if (tagNameGroup == "imagelayer")
        {
            ReadImageLayer(GroupElement);
        }
        else if (tagNameGroup == "group")
        {
            GroupTiled* tempGroupTiled = new GroupTiled();
            tempGroupTiled->ReadGroup(GroupElement, p_map);
			m_groups.push_back(tempGroupTiled);
        }
        GroupElement = GroupElement->NextSiblingElement();
    }
}

void GroupTiled::ReadGroupProperties(tinyxml2::XMLElement* p_groupElement)
{
    tinyxml2::XMLElement* propertiesElement = p_groupElement->FirstChildElement();
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

GroupTiled::GroupTiled()
{
    m_properties.reserve(8);     // quelques propriétés custom par groupe
    m_groups.reserve(4);         // groupes imbriqués, rarement beaucoup
    m_layers.reserve(8);         // layers de tuiles par groupe
    m_objectGroups.reserve(4);   // groupes d'objets par groupe
    m_imageLayers.reserve(4);    // image layers par groupe
}

GroupTiled::~GroupTiled()
{
    for (PropertyTiled* prop : m_properties)
    {
        delete prop;
    }
    m_properties.clear();
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

PropertyTiled* GroupTiled::GetGroupProperty(const std::string& p_propertyName) const
{
    auto it = std::find_if(
        m_properties.begin(), m_properties.end(),
        [&p_propertyName](const PropertyTiled* prop)
        {
            return prop->m_PropertyName == p_propertyName;
        });
    return it != m_properties.end() ? *it : nullptr;
}

GroupTiled* GroupTiled::GetGroupByName(const std::string& p_name)
{
    for (GroupTiled* group : m_groups)
    {
        if (group->m_GroupName == p_name)
        {
            return group;
        }
	}
}
LayerTiled* GroupTiled::GetLayerByName(const std::string& p_name)
{
    for (LayerTiled* layer : m_layers)
    {
        if (layer->m_LayerName == p_name)
        {
            return layer;
        }
	}
}

ObjectGroupTiled* GroupTiled::GetObjectGroupByName(const std::string& p_name)
{
    for (ObjectGroupTiled* objectGroup : m_objectGroups)
    {
        if (objectGroup->m_ObjectGroupName == p_name)
        {
            return objectGroup;
        }
    }
}

ImageLayerTiled* GroupTiled::GetImageLayerByName(const std::string& p_name)
{
    for (ImageLayerTiled* imageLayer : m_imageLayers)
    {
        if (imageLayer->m_ImageLayerName == p_name)
        {
            return imageLayer;
        }
	}
}