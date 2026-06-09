#include "pch.h"
#include "ParserSpriteSheet.h"
#include <fstream>
#include "json.hpp"
#include <iostream>

//ParserSpriteSheet* ParserSpriteSheet::Get()
//{
//    static ParserSpriteSheet instance;
//    return &instance;
//}


void ParserSpriteSheet::ReadJson(const std::string& p_path)
{
  //  std::ifstream file(p_path);
  //  if (!file.is_open())
  //  {
  //      std::cerr << "Json file for animation " << p_path << " not found" << std::endl;
  //  }
  //  else
  //  {
  //      std::cerr << "Json file for animation " << p_path << " loaded successfully" << std::endl;
  //  }

  //  nlohmann::json spriteSheet_json;
  //  file >> spriteSheet_json;

  //  if (spriteSheet_json.contains("frame_size") == false)
  //      return;

  //  for (auto& [key, value] : spriteSheet_json["frame_size"].items())
  //  {
  //      m_frameDimension.GetX() = spriteSheet_json["frame_size"]["width"].get<int>();
	 //   m_frameDimension.GetY() = spriteSheet_json["frame_size"]["height"].get<int>();
  //  }
  // 
  //  if (spriteSheet_json.contains("animations") == false)
		//return;

  //  for (auto& [animation, data] : spriteSheet_json["animations"].items())
  //  {
  //      InfoAnimation animations;

  //      animations.s_name = animation;
  //      animations.s_nbFrames = data["frames"].get<int>();
  //      animations.row = data["row"].get<int>();
  //      animations.s_loop = data["loop"].get<bool>();

  //      m_animations.push_back(animations);
  //  }
}

void ParserSpriteSheet::ReadJsonYuki(const std::string& p_path)
{
    std::ifstream file(p_path);

    if (!file.is_open())
    {
        std::cerr << "Json file for animation " << p_path << " not found" << std::endl;
        return;
    }

    std::cerr << "Json file for animation " << p_path << " loaded successfully" << std::endl;

    nlohmann::json spriteSheet_json;
    file >> spriteSheet_json;

    if (spriteSheet_json.contains("animations") == false)
        return;

    for (auto& [animation, data] : spriteSheet_json["animations"].items())
    {
        InfoAnimation animations;

        animations.s_nameAnimation = animation;
		animations.s_frameTime = data["frame_time"].get<float>();
        animations.s_nbFrames = data["frames"].get<std::vector<int>>();
		animations.s_loop = data["loop"].get<bool>(); 

		m_animations.push_back(animations);
    }

    if (spriteSheet_json.contains("frame_size") == false)
        return;

    for (auto& [dimension, value] : spriteSheet_json["frame_size"].items())
    {
        m_frameDimension.GetX() = spriteSheet_json["frame_size"]["width"].get<int>();
        m_frameDimension.GetY() = spriteSheet_json["frame_size"]["height"].get<int>();
    }

    if (spriteSheet_json.contains("texture") == false)
        return;

    m_texturePath = spriteSheet_json["texture"].get<std::string>();

    std::cerr << "Json file for animation " << p_path << " read successfully" << std::endl;
}

const InfoAnimation* ParserSpriteSheet::GetInfoAnimationByName(const std::string& name) const
{
    for (auto& animation : m_animations)
    {
        if (animation.s_nameAnimation == name)
            return &animation;
    }
    return nullptr;
}

