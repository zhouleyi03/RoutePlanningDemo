/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#pragma once

#include "MapGenerator.h"
#include "Map.h"

// 修改下面这（些）参数来配置 BlockGen：
constexpr int BlockGen_block_num = 64;

class BlockGen : public MapGenerator
{
public:
    virtual void operator()(Map &map) override;
};
