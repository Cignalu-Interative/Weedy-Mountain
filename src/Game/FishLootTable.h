#pragma once
#include <random>
#include <array>
#include <cstdint>

enum class Weather : uint8_t;
enum class TimeOfDay : uint8_t;
enum class Season : uint8_t;

using WeatherMask = uint8_t;
using TimeMask = uint8_t;
using SeasonMask = uint8_t;

static constexpr WeatherMask ALL_WEATHER = 0b111;
static constexpr TimeMask ALL_TIME = 0b11;
static constexpr SeasonMask ALL_SEASON = 0b1111;

struct FishVariant
{
	std::string m_id;
	float weight;
	float m_xpOnCatch;
};

struct FishSpecies
{
	WeatherMask m_weatherMask;
	TimeMask m_timeMask;
	SeasonMask m_seasonMask;
	float m_raceWeight;
	std::array<FishVariant, 3> m_variants;
};

class FishLootTable
{
public:
	static std::string PickOne();
	static float GetXpForFish(const std::string& fishID);

private:
	static const std::array<FishSpecies, 12> GetTable();

	static int WeightedPick(const float* weights, int count, float total);

	static std::mt19937& Rng();
};

