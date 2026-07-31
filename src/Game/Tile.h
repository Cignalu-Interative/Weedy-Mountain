#pragma once
#include <variant>
#include <type_traits>
#include "FarmTile.h"
#include "CaveTile.h"

using Tile = std::variant<std::monostate, FarmTile*, CaveTile*>;

namespace TileUtils
{
	
inline bool IsSet(const Tile& tile){
	return !std::holds_alternative<std::monostate>(tile);
}


inline Vector2f GetPosition(const Tile& tile) {
	return std::visit([](auto&& t) -> Vector2f
		{
			using T = std::decay_t<decltype(t)>;
			if constexpr (std::is_same_v<T, std::monostate>)
				return Vector2f{};
			else
				return t->GetPosition();
		}, tile);
}

inline void SetCanGrow(const Tile& tile, bool value) {
	std::visit([value](auto&& t)
		{
			using T = std::decay_t<decltype(t)>;
			if constexpr (!std::is_same_v<T, std::monostate>)
				t->SetCanGrow(value);
		}, tile);
}

inline bool CanGrow(const Tile& tile)
{
	return std::visit([](auto&& t) -> bool
		{
			using T = std::decay_t<decltype(t)>;
			if constexpr (std::is_same_v<T, std::monostate>)
				return false;
			else
				return t->CanGrow();
		}, tile);
}

}