#include "pch.h"
#include "FishLootTable.h"
#include "GameData.h"

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
}

const std::array<FishSpecies, 12> FishLootTable::GetTable()
{
    static const std::array<FishSpecies, 12> table
    {
        FishSpecies{ (uint8_t)Weather::Rain | (uint8_t)Weather::Storm,  (uint8_t)TimeOfDay::Day,    ALL_SEASON,                                         50.f,   {FishVariant{"ITEM_RED_FISH",       55.f},FishVariant{"ITEM_RED_SPARKLING_FISH",        30.f},FishVariant{"ITEM_RED_KING_FISH",         15.f}}},
        FishSpecies{ (uint8_t)Weather::Clear,                           ALL_TIME,                   ALL_SEASON,                                         65.f,   {FishVariant{"ITEM_BLUE_FISH",      55.f},FishVariant{"ITEM_BLUE_SPARKLING_FISH",       30.f},FishVariant{"ITEM_BLUE_KING_FISH",        15.f}}},
        FishSpecies{ ALL_WEATHER,                                       (uint8_t)TimeOfDay::Night,  ALL_SEASON,                                         60.f,   {FishVariant{"ITEM_CYAN_FISH",      55.f},FishVariant{"ITEM_CYAN_SPARKLING_FISH",       30.f},FishVariant{"ITEM_CYAN_KING_FISH",        15.f}}},
        FishSpecies{ (uint8_t)Weather::Clear,                           (uint8_t)TimeOfDay::Day,    (uint8_t)Season::Spring | (uint8_t)Season::Summer,  55.f,   {FishVariant{"ITEM_GREEN_FISH",     55.f},FishVariant{"ITEM_GREEN_SPARKLING_FISH",      30.f},FishVariant{"ITEM_GREEN_KING_FISH",       15.f}}},
        FishSpecies{ ALL_WEATHER,                                       (uint8_t)TimeOfDay::Night,  (uint8_t)Season::Winter,                            45.f,   {FishVariant{"ITEM_BROWN_FISH",     55.f},FishVariant{"ITEM_BROWN_SPARKLING_FISH",      30.f},FishVariant{"ITEM_BROWN_KING_FISH",       15.f}}},
        FishSpecies{ (uint8_t)Weather::Rain,                            ALL_TIME,                   (uint8_t)Season::Autumn,                            40.f,   {FishVariant{"ITEM_GREY_FISH",      55.f},FishVariant{"ITEM_GREY_SPARKLING_FISH",       30.f},FishVariant{"ITEM_GREY_KING_FISH",        15.f}}},
        FishSpecies{ (uint8_t)Weather::Clear,                           (uint8_t)TimeOfDay::Day,    (uint8_t)Season::Summer,                            40.f,   {FishVariant{"ITEM_PINK_FISH",      55.f},FishVariant{"ITEM_PINK_SPARKLING_FISH",       30.f},FishVariant{"ITEM_PINK_KING_FISH",        15.f}}},
        FishSpecies{ (uint8_t)Weather::Rain,                            (uint8_t)TimeOfDay::Day,    (uint8_t)Season::Spring,                            35.f,   {FishVariant{"ITEM_LIGHTPINK_FISH", 55.f},FishVariant{"ITEM_LIGHTPINK_SPARKLING_FISH",  30.f},FishVariant{"ITEM_LIGHTPINK_KING_FISH",   15.f}}},
        FishSpecies{ (uint8_t)Weather::Storm,                           (uint8_t)TimeOfDay::Night,  ALL_SEASON,                                         20.f,   {FishVariant{"ITEM_PURPLE_FISH",    55.f},FishVariant{"ITEM_PURPLE_SPARKLING_FISH",     30.f},FishVariant{"ITEM_PURPLE_KING_FISH",      15.f}}},
        FishSpecies{ (uint8_t)Weather::Clear,                           (uint8_t)TimeOfDay::Night,  (uint8_t)Season::Summer,                            18.f,   {FishVariant{"ITEM_ORANGE_FISH",    55.f},FishVariant{"ITEM_ORANGE_SPARKLING_FISH",     30.f},FishVariant{"ITEM_ORANGE_KING_FISH",      15.f}}},
        FishSpecies{ (uint8_t)Weather::Storm,                           (uint8_t)TimeOfDay::Night,  (uint8_t)Season::Winter,                            15.f,   {FishVariant{"ITEM_BLACK_FISH",     55.f},FishVariant{"ITEM_BLACK_SPARKLING_FISH",      30.f},FishVariant{"ITEM_BLACK_KING_FISH",       15.f}}},
        FishSpecies{ (uint8_t)Weather::Storm,                           (uint8_t)TimeOfDay::Night,  (uint8_t)Season::Autumn,                            6.f,    {FishVariant{"ITEM_GOLDEN_FISH",    70.f},FishVariant{"ITEM_GOLDEN_SPARKLING_FISH",     20.f},FishVariant{"ITEM_GOLDEN_KING_FISH",      10.f}}},
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

    return -1;
}

std::mt19937& FishLootTable::Rng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}
