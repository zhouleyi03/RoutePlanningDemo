#pragma once

#include "Algo.h"

constexpr int DFS_frame_interval = 1;

class DFS : public Algo
{
public:
    DFS(int *frame_var, bool *end_flag);

    virtual void exec() override;
};
