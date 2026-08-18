#pragma once
#include <vector>
#include <unordered_map>
#include "GameData.h"

using WeatherMask = uint8_t;
using TimeMask = uint8_t;
using SeasonMask = uint8_t; 

constexpr WeatherMask W(Weather w) { return 1 << (uint8_t)w; }
constexpr TimeMask T(TimeOfDay t) { return 1 << (uint8_t)t; }
constexpr SeasonMask S(Season s) { return 1 << (uint8_t)s; }

struct CropDefinition
{
	std::string m_name;
	std::vector<std::string> m_growthSpritePath;
	SeasonMask m_seasonMask;
	int m_daysToGrow;
	int m_daysWithoutWaterBeforeDeath;
	float m_xpOnHarvest;
	int m_dropCountMin;
	int m_dropCountMax;
};
namespace CropCatalog
{
	inline CropDefinition Wheat()
	{
		CropDefinition def;
		def.m_name = "Wheat";
		def.m_growthSpritePath = {
			"wheat_01", "wheat_02", "wheat_03", "wheat_04"
		};
		def.m_seasonMask = S(Season::Summer) | S(Season::Autumn);
		def.m_daysToGrow = 4;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 10;
		def.m_dropCountMin = 2;
		def.m_dropCountMax = 3;
		return def;
	}
	inline CropDefinition Carrot()
	{
		CropDefinition def;
		def.m_name = "Carrot";
		def.m_growthSpritePath = {
			"carrot_01", "carrot_02", "carrot_03", "carrot_04"
		};
		def.m_seasonMask = S(Season::Spring) | S(Season::Autumn);
		def.m_daysToGrow = 6;
		def.m_daysWithoutWaterBeforeDeath = 2;
		def.m_xpOnHarvest = 15;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 3;
		return def;
	}
	inline CropDefinition Turnip()
	{
		CropDefinition def;
		def.m_name = "Turnip";
		def.m_growthSpritePath = {
			"turnip_01", "turnip_02", "turnip_03", "turnip_04"
		};
		def.m_seasonMask = S(Season::Spring);
		def.m_daysToGrow = 5;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 10;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 2;
		return def;
	}
	inline CropDefinition Pumpkin()
	{
		CropDefinition def;
		def.m_name = "Pumpkin";
		def.m_growthSpritePath = {
			"pumpkin_01", "pumpkin_02", "pumpkin_03", "pumpkin_04"
		};
		def.m_seasonMask = S(Season::Autumn);
		def.m_daysToGrow = 13;
		def.m_daysWithoutWaterBeforeDeath = 4;
		def.m_xpOnHarvest = 28;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 1;
		return def;
	}
	inline CropDefinition Beatroot()
	{
		CropDefinition def;
		def.m_name = "Beatroot";
		def.m_growthSpritePath = {
			"beatroot_01", "beatroot_02", "beatroot_03", "beatroot_04"
		};
		def.m_seasonMask = S(Season::Summer);
		def.m_daysToGrow = 9;
		def.m_daysWithoutWaterBeforeDeath = 2;
		def.m_xpOnHarvest = 18;
		def.m_dropCountMin = 2;
		def.m_dropCountMax = 4;
		return def;
	}
	inline CropDefinition Parsnip()
	{
		CropDefinition def;
		def.m_name = "Parsnip";
		def.m_growthSpritePath = {
			"parsnip_01", "parsnip_02", "parsnip_03", "parsnip_04"
		};
		def.m_seasonMask = S(Season::Spring);
		def.m_daysToGrow = 6;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 12;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 3;
		return def;
	}
	inline CropDefinition Potato()
	{
		CropDefinition def;
		def.m_name = "Potato";
		def.m_growthSpritePath = {
			"potato_01", "potato_02", "potato_03", "potato_04"
		};
		def.m_seasonMask = S(Season::Autumn);
		def.m_daysToGrow = 8;
		def.m_daysWithoutWaterBeforeDeath = 4;
		def.m_xpOnHarvest = 16;
		def.m_dropCountMin = 2;
		def.m_dropCountMax = 4;
		return def;
	}
	inline CropDefinition Radish()
	{
		CropDefinition def;
		def.m_name = "Radish";
		def.m_growthSpritePath = {
			"radish_01", "radish_02", "radish_03", "radish_04"
		};
		def.m_seasonMask = S(Season::Spring) | S(Season::Summer);
		def.m_daysToGrow = 6;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 13;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 2;
		return def;
	}
	inline CropDefinition Eggplant()
	{
		CropDefinition def;
		def.m_name = "Eggplant";
		def.m_growthSpritePath = {
			"eggplant_01", "eggplant_02", "eggplant_03", "eggplant_04"
		};
		def.m_seasonMask = S(Season::Summer) | S(Season::Autumn);
		def.m_daysToGrow = 10;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 20;
		def.m_dropCountMin = 2;
		def.m_dropCountMax = 3;
		return def;
	}
	inline CropDefinition Cabbage()
	{
		CropDefinition def;
		def.m_name = "Cabbage";
		def.m_growthSpritePath = {
			"cabbage_01", "cabbage_02", "cabbage_03", "cabbage_04"
		};
		def.m_seasonMask = S(Season::Spring) | S(Season::Autumn);
		def.m_daysToGrow = 7;
		def.m_daysWithoutWaterBeforeDeath = 4;
		def.m_xpOnHarvest = 14;
		def.m_dropCountMin = 1;
		def.m_dropCountMax = 1;
		return def;
	}
	inline const std::unordered_map<std::string, CropDefinition>& GetAll()
	{
		static std::unordered_map<std::string, CropDefinition> all =
		{
			{ "Wheat", Wheat() },
			{ "Carrot", Carrot() },
			{ "Turnip", Turnip() },
			{ "Pumpkin", Pumpkin() },
			{ "Beatroot", Beatroot() },
			{ "Parsnip", Parsnip() },
			{ "Potato", Potato() },
			{ "Radish", Radish() },
			{ "Eggplant", Eggplant() },
			{ "Cabbage", Cabbage() }
		};
		return all;
	}
	inline const CropDefinition* Get(const std::string& p_name)
	{
		auto& all = GetAll();
		auto it = all.find(p_name);
		return it != all.end() ? &it->second : nullptr;
	}
}