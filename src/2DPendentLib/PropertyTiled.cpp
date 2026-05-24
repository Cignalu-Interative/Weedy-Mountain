#include "pch.h"
#include "PropertyTiled.h"
#include "MapTiled.h"
#include "TilesetTiled.h"
#include "ObjectTiled.h"
#include "TileTiled.h"
#include "LayerTiled.h"
#include "GroupTiled.h"
#include "ObjectGroupTiled.h"
#include "ImageLayerTiled.h"

void PropertyTiled::ReadPropertyValue(std::string p_valueStr)
{
    if (m_PropertyType == "bool")
    {
        // Convertit la chaîne de caractères en booléen (true/false)
        // Ex: valueStr = "true" => s_PropertyValue = true
        // Ex: valueStr = "false" => s_PropertyValue = false
        m_PropertyValue = (p_valueStr == "true");
    }
    else if (m_PropertyType == "color")
    {
        std::string colorStr = p_valueStr.substr(1);  // Enlève #

        if (colorStr.size() == 8)
        {  // #AARRGGBB
            try
            {
                uint32_t hex = std::stoul(colorStr, nullptr, 16);
                m_PropertyValue = Color(
                    (hex >> 16) & 0xFF,  // R
                    (hex >> 8) & 0xFF,  // G  
                    hex & 0xFF    // B
                );
            }
            catch (...)
            {
                m_PropertyValue = ColorsNamed::White;
            }
        }
        else if (colorStr.size() == 6)
        {  // #RRGGBB
            try
            {
                uint32_t hex = std::stoul(colorStr, nullptr, 16);
                m_PropertyValue = Color(
                    (hex >> 16) & 0xFF,  // R
                    (hex >> 8) & 0xFF,  // G  
                    hex & 0xFF,   // B
                    255                 // Alpha=opaque
                );
            }
            catch (...)
            {
                m_PropertyValue = ColorsNamed::White;
            }
        }
        else
        {
            m_PropertyValue = ColorsNamed::White;
        }
    }
    else if (m_PropertyType == "float")
    {
        try
        {
            m_PropertyValue = std::stof(p_valueStr);
        }
        catch (...)
        {
            m_PropertyValue = 0.0f;
        }
    }
    else if (m_PropertyType == "int")
    {
        try
        {
            m_PropertyValue = std::stoi(p_valueStr);
        }
        catch (...)
        {
            m_PropertyValue = 0;
        }
    }
    else if (m_PropertyType == "object")
    {
        try
        {
            m_PropertyValue = std::stoi(p_valueStr);
        }
        catch (...)
        {
            m_PropertyValue = 0;
        }
    }
    else if (m_PropertyType == "string" || m_PropertyType == "file")
    {
        m_PropertyValue = p_valueStr; // Stocke la valeur de la chaîne
    }
}

bool PropertyTiled::HasValue() const
{
    return m_PropertyName != "" && m_PropertyType != "";
}
