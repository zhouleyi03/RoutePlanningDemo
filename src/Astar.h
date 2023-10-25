#pragma once

#include "Algo.h"

constexpr int Astar_frame_interval = 1;

class Astar : public Algo
{
public:
    Astar(int *frame_var, bool *end_flag);

    virtual void exec() override;
};
