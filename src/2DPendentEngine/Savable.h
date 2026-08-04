#pragma once
#include "2DpendentLib/json.hpp"

using json = nlohmann::json;

class Savable
{
public:
	virtual ~Savable() = default;

	virtual std::string GetSaveID() const = 0;

	virtual void Save(json& p_out) = 0;
	virtual void Load(const json& p_in) = 0;
};

