#pragma once
#include "2DPendentLib/Vector2.h"
#include <string>
#include <vector>

struct InfoAnimation
{
	std::string s_nameAnimation;
	std::vector<int> s_nbFrames;
	float s_frameTime;
	//int row;
	bool s_loop;

	InfoAnimation()
	{
		s_nameAnimation = "default";
		s_nbFrames.reserve(10);
		s_frameTime = 1.0f;
		s_loop = true;
	}
};

class ParserSpriteSheet
{
	Vector2i m_frameDimension; // x = width, y = height
	//int m_structure;

	std::vector<InfoAnimation> m_animations;

	std::string m_texturePath;


public :

	ParserSpriteSheet();

	//static ParserSpriteSheet* Get();

	void ReadJson(const std::string& p_path);

	void ReadJsonYuki(const std::string& p_path);

	const InfoAnimation* GetInfoAnimationByName(const std::string& name) const;
	Vector2i GetFrameDimension() const { return m_frameDimension; }
};

