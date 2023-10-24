/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#pragma once

enum class GridState : unsigned char
{
    EMPTY,
    BARRIER,
    VISITED,
    PENDING,
    STARTPOINT,
    ENDPOINT,
};
