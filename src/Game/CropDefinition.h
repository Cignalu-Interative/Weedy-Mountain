#pragma once
#include <vector>

struct CropDefinition
{
	std::string m_name;

	std::vector<std::string> m_growthSpritePath;

	int m_daysToGrow;
	int m_daysWithoutWaterBeforeDeath;
	float m_xpOnHarvest;
	int m_dropCount;
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
		def.m_daysToGrow = 4;
		def.m_daysWithoutWaterBeforeDeath = 3;
		def.m_xpOnHarvest = 10;
		def.m_dropCount = 3;

		return def;
	}

	inline CropDefinition Carrot()
	{
		CropDefinition def;

		def.m_name = "Carrot";
		def.m_growthSpritePath = {
			"carrot_01", "carrot_02", "carrot_03", "carrot_04"
		};
		def.m_daysToGrow = 6;
		def.m_daysWithoutWaterBeforeDeath = 2;
		def.m_xpOnHarvest = 15;
		def.m_dropCount = 2;

		return def;
	}
}

