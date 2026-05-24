#pragma once
#include <string>
#include <variant>
#include <unordered_map>
#include "Color.h"
#include "tinyxml2.h"

//bool ->bool
//sf::Color -> color
//float -> float
//int -> int et object (ex: Entity* ou MapTiled*)
//std::string -> string et file(chemin de fichier)
using PropertyValue = std::variant<bool, Color, float, int, std::string>;

class MapTiled;
class TilesetTiled;
class ObjectTiled;
class TileTiled;
class LayerTiled;
class GroupTiled;
class ObjectGroupTiled;
class ImageLayerTiled;
class PropertyTiled
{
private:

	std::string m_PropertyName; // Le nom de la propriété
	std::string m_PropertyType; // Le type de la propriété (bool, color, float, int, object, string)

	PropertyValue m_PropertyValue; // La valeur de la propriété, qui peut être de différents types grâce à std::variant

public:

	friend class MapTiled;
	friend class TilesetTiled;
	friend class ObjectTiled;
	friend class TileTiled;
	friend class LayerTiled;
	friend class GroupTiled;
	friend class ObjectGroupTiled;
	friend class ImageLayerTiled;

	void ReadPropertyValue(std::string p_value);

	std::string GetPropertyName() const { return m_PropertyName; }
	std::string GetPropertyType() const { return m_PropertyType; }

	bool HasValue() const;

	template<typename T>
	T GetPropertyValue() const;
};

#include "PropertyTiled.inl"