#pragma once

#include "Algo.h"

class BFS : public Algo
{
public:
    virtual void operator()(Map &map, std::atomic_int &frame) override;
};
