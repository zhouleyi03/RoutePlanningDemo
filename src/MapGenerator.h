/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#pragma once

#include "Map.h"

#include <memory>

class MapGenerator
{
public:
    ~MapGenerator();

    void genSEPoint(Map &map);
    virtual void operator()(Map &map) = 0;
};
