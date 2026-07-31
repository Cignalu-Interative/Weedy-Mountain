#pragma once
#include <json.hpp>

using json = nlohmann::json;

class Savable
{
public:
	virtual ~Savable() = default;

	virtual std::string GetSaveID() = 0;

	virtual void Save(json& p_out) = 0;
	virtual void Load(const json& p_in) = 0;
};

