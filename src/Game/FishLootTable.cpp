#include "pch.h"
#include "FishLootTable.h"
#include "GameData.h"

constexpr WeatherMask W(Weather w) { return 1 << (uint8_t)w; }
constexpr TimeMask T(TimeOfDay t) { return 1 << (uint8_t)t; }
constexpr SeasonMask S(Season s) { return 1 << (uint8_t)s; }

std::string FishLootTable::PickOne()
{
    GameData& gm = GameData::Get();

    Weather weather = gm.GetWeather();
    TimeOfDay time = gm.GetTimeOfDay();
    Season season = gm.GetSeason();

    WeatherMask wBit = 1 << (uint8_t)weather;
    TimeMask tBit = 1 << (uint8_t)time;
    SeasonMask sBit = 1 << (uint8_t)season;

    const auto& table = GetTable();

    std::array<const FishSpecies*, 12> eligible{};
    std::array<float, 12> eligibleWeights{};
    int eligibleCount = 0;
    float totalWeight = 0.f;

    for (const FishSpecies& species : table)
    {
        bool weatherOk = species.m_weatherMask & wBit;
        bool timeOk = species.m_timeMask & tBit;
        bool seasonOk = species.m_seasonMask & sBit;

        if (weatherOk && timeOk && seasonOk)
        {
            eligible[eligibleCount] = &species;
            eligibleWeights[eligibleCount] = species.m_raceWeight;
            totalWeight += species.m_raceWeight;
            ++eligibleCount;
        }
    }

    if (eligibleCount == 0)
        return "ITEM_NOTHING"; 

    int raceIndex = WeightedPick(eligibleWeights.data(), eligibleCount, totalWeight);
    const FishSpecies* chosenSpecies = eligible[raceIndex];

    float variantWeights[3] = {
        chosenSpecies->m_variants[0].weight,
        chosenSpecies->m_variants[1].weight,
        chosenSpecies->m_variants[2].weight
    };

    float variantTotal = variantWeights[0] + variantWeights[1] + variantWeights[2];

    int variantIndex = WeightedPick(variantWeights, 3, variantTotal);

    return chosenSpecies->m_variants[variantIndex].m_id;
}

const std::array<FishSpecies, 12> FishLootTable::GetTable()
{
    static const std::array<FishSpecies, 12> table
    {
        FishSpecies{ W(Weather::Rain) | W(Weather::Storm),  T(TimeOfDay::Day),      ALL_SEASON,                             50.f,   {FishVariant{"ITEM_RED_FISH",       55.f},FishVariant{"ITEM_RED_SPARKLING_FISH",        30.f},FishVariant{"ITEM_RED_KING_FISH",         15.f}}},
        FishSpecies{ W(Weather::Clear),                     ALL_TIME,               ALL_SEASON,                             65.f,   {FishVariant{"ITEM_BLUE_FISH",      55.f},FishVariant{"ITEM_BLUE_SPARKLING_FISH",       30.f},FishVariant{"ITEM_BLUE_KING_FISH",        15.f}}},
        FishSpecies{ ALL_WEATHER,                           T(TimeOfDay::Night),    ALL_SEASON,                             60.f,   {FishVariant{"ITEM_CYAN_FISH",      55.f},FishVariant{"ITEM_CYAN_SPARKLING_FISH",       30.f},FishVariant{"ITEM_CYAN_KING_FISH",        15.f}}},
        FishSpecies{ W(Weather::Clear),                     T(TimeOfDay::Day),      S(Season::Spring ) | S(Season::Summer), 55.f,   {FishVariant{"ITEM_GREEN_FISH",     55.f},FishVariant{"ITEM_GREEN_SPARKLING_FISH",      30.f},FishVariant{"ITEM_GREEN_KING_FISH",       15.f}}},
        FishSpecies{ ALL_WEATHER,                           T(TimeOfDay::Night),    S(Season::Winter),                      45.f,   {FishVariant{"ITEM_BROWN_FISH",     55.f},FishVariant{"ITEM_BROWN_SPARKLING_FISH",      30.f},FishVariant{"ITEM_BROWN_KING_FISH",       15.f}}},
        FishSpecies{ W(Weather::Rain),                      ALL_TIME,               S(Season::Autumn),                      40.f,   {FishVariant{"ITEM_GRAY_FISH",      55.f},FishVariant{"ITEM_GRAY_SPARKLING_FISH",       30.f},FishVariant{"ITEM_GRAY_KING_FISH",        15.f}}},
        FishSpecies{ W(Weather::Clear),                     T(TimeOfDay::Day),      S(Season::Summer),                      40.f,   {FishVariant{"ITEM_PINK_FISH",      55.f},FishVariant{"ITEM_PINK_SPARKLING_FISH",       30.f},FishVariant{"ITEM_PINK_KING_FISH",        15.f}}},
        FishSpecies{ W(Weather::Rain),                      T(TimeOfDay::Day),      S(Season::Spring),                      35.f,   {FishVariant{"ITEM_LIGHTPINK_FISH", 55.f},FishVariant{"ITEM_LIGHTPINK_SPARKLING_FISH",  30.f},FishVariant{"ITEM_LIGHTPINK_KING_FISH",   15.f}}},
        FishSpecies{ W(Weather::Storm),                     T(TimeOfDay::Night),    ALL_SEASON,                             20.f,   {FishVariant{"ITEM_PURPLE_FISH",    55.f},FishVariant{"ITEM_PURPLE_SPARKLING_FISH",     30.f},FishVariant{"ITEM_PURPLE_KING_FISH",      15.f}}},
        FishSpecies{ W(Weather::Clear),                     T(TimeOfDay::Night),    S(Season::Summer),                      18.f,   {FishVariant{"ITEM_ORANGE_FISH",    55.f},FishVariant{"ITEM_ORANGE_SPARKLING_FISH",     30.f},FishVariant{"ITEM_ORANGE_KING_FISH",      15.f}}},
        FishSpecies{ W(Weather::Storm),                     T(TimeOfDay::Night),    S(Season::Winter),                      15.f,   {FishVariant{"ITEM_BLACK_FISH",     55.f},FishVariant{"ITEM_BLACK_SPARKLING_FISH",      30.f},FishVariant{"ITEM_BLACK_KING_FISH",       15.f}}},
        FishSpecies{ W(Weather::Storm),                     T(TimeOfDay::Night),    S(Season::Autumn),                      6.f,    {FishVariant{"ITEM_GOLDEN_FISH",    70.f},FishVariant{"ITEM_GOLDEN_SPARKLING_FISH",     20.f},FishVariant{"ITEM_GOLDEN_KING_FISH",      10.f}}},
    };

    return table;
}

int FishLootTable::WeightedPick(const float* weights, int count, float total)
{
    std::uniform_real_distribution<float> dist(0.f, total);
    float r = dist(Rng());

    float cumultative = 0.f;
    for (int i = 0; i < count; i++)
    {
        cumultative += weights[i];

        if (r <= cumultative)
            return i;
    }

    return count - 1;
}

std::mt19937& FishLootTable::Rng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}
