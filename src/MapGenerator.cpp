/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#include "MapGenerator.h"

#include "Map.h"

#include <random>
#include <ctime>

MapGenerator::~MapGenerator()
{
}

void MapGenerator::genSEPoint(Map &map)
{
    int map_size = map.getData().size();
    int map_size_root = std::sqrt(map_size);

    std::default_random_engine engine;
    std::uniform_int_distribution<int> dist(0, map_size);
    engine.seed(static_cast<unsigned>(std::time(nullptr)));

    int s, e;
    do
    {
        s = dist(engine);
    } while (map.getGridState(s) != GridState::EMPTY);
    map.setGridState(s, GridState::STARTPOINT);

    do
    {
        e = dist(engine);
    } while (map.getGridState(e) != GridState::EMPTY && abs(e - s) > (map_size_root << 2));
    map.setGridState(e, GridState::ENDPOINT);
}
