#pragma once

#include "Algo.h"

constexpr int BFS_frame_interval = 5;

class BFS : public Algo
{
public:
    virtual void exec(Map &map, std::atomic_int &frame_var) override;
};
